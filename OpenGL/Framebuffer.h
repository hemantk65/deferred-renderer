#pragma once

#include <GL\glew.h>
#include <vector>

struct sAttachment
{
	GLenum location;
	GLuint buffer;
};

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void genFbo(std::vector<sAttachment> attachments);
	void useFbo() { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); };
	GLuint getFbo() { return m_fbo; };
	
private:
	GLuint m_fbo;
};