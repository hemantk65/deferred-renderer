#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
	m_fbo = 0;
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_fbo);
}

void Framebuffer::genFbo(std::vector<sAttachment> attachments)
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	std::vector<GLenum> drawBufs;
	for (auto attachment : attachments)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment.location, GL_TEXTURE_2D, attachment.buffer, 0);
		if ((attachment.location != GL_DEPTH_ATTACHMENT) && (attachment.location != GL_STENCIL_ATTACHMENT) && (attachment.location != GL_DEPTH_STENCIL_ATTACHMENT))
			drawBufs.push_back(attachment.location);
	}
	glDrawBuffers(drawBufs.size(), drawBufs.data());
}