#pragma once
#include "RenderObject.h"
#include <memory>
#include <glm/glm.hpp>
#include <map>

#include "GLRenderingProgram.h"
#include "GLTexture.h"

class Character : public RenderObject
{
public:
	Character(char c, std::shared_ptr<GLRenderingProgram> prog, 
		unsigned fontSize = Character::s_currentFontSize);

	virtual void draw(const glm::mat4& matrix);
	void setup();

	void setLoc(glm::vec2 pos, float scale = 1.0f) { _pos = pos; _scale = scale; }
	void setColor(glm::vec3 color) { _color = color; }

	char getChar() const { return _char; }

	static GLuint getAdvance(char c) { return Character::s_charData[c].advance; }
	static GLuint getAdvanceY() { return Character::s_advanceY; }
	static unsigned getFontSize() { return s_currentFontSize; }
	static void generateLookupTable(unsigned fontSize);

private:
	glm::vec2 _pos;
	float _scale;
	char _char;
	glm::vec3 _color;

	//Texture atlas data shared by all characters
	static GLTexture s_tex;
	static bool s_lookupCreated;
	static unsigned s_currentFontSize;
	static GLuint s_advanceY;

	static struct charData
	{
		glm::ivec2 size;
		glm::ivec2 bearing;
		GLuint advance;

		float textureOffset;
	} s_charData[128];
	
};
