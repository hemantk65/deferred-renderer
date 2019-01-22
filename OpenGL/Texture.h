#pragma once

#include <GL/glew.h>

class Texture
{
public:
	Texture();
	~Texture();

	void loadTexture(const char* name);
	void genTexture(GLenum internalFormat, GLenum format, GLint width, GLint height, GLenum type, void* data);
	GLuint* getTexture() { return &m_tex; };

private:
	GLuint m_tex;
};