#pragma once

#include <GL/glew.h>

class Shader
{
public:
	Shader();
	Shader(const char*, const char*);
	~Shader();

	void loadShaders(const char*, const char*);

	GLuint getProgram() { return m_shader; };

	void useProgram() { glUseProgram(m_shader); };

private:
	GLuint m_shader;
};