#pragma once

#include <GL/glew.h>

#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	void loadTexture(const char* name, std::string type);
	void genTexture(GLenum internalFormat, GLenum format, GLint width, GLint height, GLenum type, void* data);
	GLuint* getTexture() { return &m_tex; };
	std::string getTextureType() { return m_type; };

private:
	GLuint m_tex;
	std::string m_type;
};