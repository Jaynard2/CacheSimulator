#include "Character.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "FreeTypeException.h"
#include "Debug.h"

#undef max

GLTexture Character::s_tex = GLTexture();
bool Character::s_lookupCreated = false;
Character::charData Character::s_charData[128];
unsigned Character::s_currentFontSize = 0;
GLuint Character::s_advanceY = 0;

Character::Character(char c, std::shared_ptr<GLRenderingProgram> prog, unsigned fontSize) 
	: RenderObject(prog), _char(c), _scale(1.0f), _color(1.0f)
{

	if (!Character::s_lookupCreated || fontSize != Character::s_currentFontSize)
		Character::generateLookupTable(fontSize);
}

void Character::draw(const glm::mat4& matrix)
{
	RenderObject::draw(matrix);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CW);

	glm::mat4 charPos = glm::translate
	(
		glm::mat4(1.0f),
		glm::vec3(_pos, 0.0f) + glm::vec3
		(
			Character::s_charData[_char].bearing.x, 
			Character::s_charData[_char].bearing.y - Character::s_charData[_char].size.y, 
			0.0f
		) * _scale
	);

	glUniform3f(p_prog->getUniformLoc("textColor"), _color.r, _color.g, _color.b);
	glUniformMatrix4fv(p_prog->getUniformLoc("proj"), 1, GL_FALSE, glm::value_ptr(matrix));
	glUniform1f(p_prog->getUniformLoc("texOffset"), s_charData[_char].textureOffset / s_tex.getWidth());
	glUniformMatrix4fv(p_prog->getUniformLoc("translationMat"), 1, GL_FALSE, glm::value_ptr(charPos));

	glActiveTexture(GL_TEXTURE0);

	s_tex.bindTexture(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, p_vbos[0]);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, p_verts.size() / 4);
}

void Character::setup()
{
	auto data = s_charData[_char];

	//Generate texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	s_tex.genTextures();
	s_tex.bindTexture(GL_TEXTURE_2D);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, s_tex.getWidth(),
		s_tex.getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, &s_tex.getData()[0]);

	//Texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::vec2 pos = _pos + glm::vec2(data.bearing.x, data.bearing.y - data.size.y) * _scale;
	glm::vec2 size = glm::vec2(data.size) * _scale;

	float height = (float)data.size.y / (float)s_tex.getHeight();

	p_verts = {
		0, 0,			0.0f / s_tex.getWidth(), height,
		0, size.y,		0.0f / s_tex.getWidth(), 0.0f,
		size.x, 0,	(float)data.size.x / s_tex.getWidth(), height,

		size.x, 0,	(float)data.size.x / s_tex.getWidth(), height,
		0, size.y,		0.0f / s_tex.getWidth(), 0.0f,
		size.x, size.y,	(float)data.size.x / s_tex.getWidth(), 0.0f
	};

	glBindVertexArray(p_vao);

	p_vbos.push_back(GLuint());
	glGenBuffers(1, &p_vbos[0]);

	glBindBuffer(GL_ARRAY_BUFFER, p_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, &p_verts[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

}

void Character::generateLookupTable(unsigned fontSize)
{

	Character::s_currentFontSize = fontSize;

	//Init FreeType
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		throw FreeTypeException("Could not initialize FreeType");

	FT_Face face;
	if (FT_New_Face(ft, "C:/Windows/Fonts/Consola.ttf", 0, &face))
		throw FreeTypeException("Failed to load font");

	FT_Set_Pixel_Sizes(face, 0, fontSize);

	unsigned textureHeight = 0;
	unsigned textureWidth = 0;
	unsigned advY = 0;
	for (char c = 32; c < 127; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			throw FreeTypeException("Failed to load glyph " + c);

		textureWidth += face->glyph->bitmap.width;
		textureHeight = std::max(textureHeight, face->glyph->bitmap.rows);
		advY = std::max(advY, (unsigned)face->glyph->bitmap.rows);
	}

	Character::s_advanceY = advY;

	std::vector<unsigned char> texAtlas;
	unsigned textureOffset = 0;

	texAtlas.resize(textureHeight * textureWidth);

	for (char c = 32; c < 127; c++)
	{
		//Load character
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			throw FreeTypeException("Failed to load glyph " + c);

		unsigned width = face->glyph->bitmap.width;
		unsigned char* buf = face->glyph->bitmap.buffer;
		unsigned int length = width * face->glyph->bitmap.rows;

		for (unsigned i = 0; i < length; i++)
			texAtlas[textureOffset + i % width + textureWidth * (i / width)] = buf[i];
		
		s_charData[c].size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		s_charData[c].bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		s_charData[c].advance = face->glyph->advance.x;
		s_charData[c].textureOffset = textureOffset;

		textureOffset += face->glyph->bitmap.width;
		
	}

	s_tex.setTextureData(texAtlas, textureWidth, textureHeight);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	s_lookupCreated = true;
}
