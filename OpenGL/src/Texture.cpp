#include "Texture.h"

#include <SOIL.h>
#include <sstream>

Texture::Texture()
{
	m_tex = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_tex);
}

void Texture::genTexture(GLenum internalFormat, GLenum format, GLint width, GLint height, GLenum type, void* data)
{
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::loadTexture(const char* name)
{
	std::stringstream ss;
	ss << "resources/" << name;
	int width, height, channels;
	unsigned char *image = SOIL_load_image
	(
		ss.str().c_str(),
		&width, &height, &channels,
		SOIL_LOAD_AUTO
	);
	GLuint format, internalFormat;
	if (channels == 3) {
		format = GL_RGB; internalFormat = GL_RGB8;
	}
	else if (channels == 4) {
		format = GL_RGBA; internalFormat = GL_RGBA8;
	}

	genTexture(internalFormat, format, width, height, GL_UNSIGNED_BYTE, image);

	delete image;
}