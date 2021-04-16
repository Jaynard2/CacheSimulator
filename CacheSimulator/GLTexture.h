#pragma once
#include <vector>
#include <gl/glew.h>

class GLTexture
{
public:
	GLTexture(const std::vector<unsigned char>& data, unsigned width, unsigned height);
	GLTexture();

	void setTextureData(const std::vector<unsigned char>& data, unsigned width, unsigned height);

	void genTextures();
	void bindTexture(GLenum glTextureName) const;

	unsigned getWidth() const { return _texWidth; }
	unsigned getHeight() const { return _texHeight; }
	const std::vector<unsigned char>& getData() const { return _texData; }
	GLuint getID() const { return _texID; }

private:
	unsigned _texWidth;
	unsigned _texHeight;
	std::vector<unsigned char> _texData;
	GLuint _texID;
};

