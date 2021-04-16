#include "GLTexture.h"

GLTexture::GLTexture(const std::vector<unsigned char>& data, unsigned width, unsigned height) : _texData(data), _texWidth(width), _texHeight(height)
{
}

GLTexture::GLTexture()
{
}

void GLTexture::setTextureData(const std::vector<unsigned char>& data, unsigned width, unsigned height)
{
	_texData = data;

	_texWidth = width;
	_texHeight = height;
}

void GLTexture::genTextures()
{
	glGenTextures(1, &_texID);
}

void GLTexture::bindTexture(GLenum glTextureName) const
{
	glBindTexture(glTextureName, _texID);
}
