#include "Blur.h"

Blur::Blur(GLuint *color_buffer, Mesh *mesh)
	: m_color_buffer(color_buffer), m_mesh(mesh)
{
}

Blur::~Blur()
{
}

void Blur::init()
{
	m_blurShader.loadShaders("blur.vert", "blur.frag");

	m_v_blur_buffer.genTexture(GL_RGBA8, GL_RGBA, WIDTH, HEIGHT, GL_UNSIGNED_BYTE, nullptr);
	m_h_blur_buffer.genTexture(GL_RGBA8, GL_RGBA, WIDTH, HEIGHT, GL_UNSIGNED_BYTE, nullptr);

	{
		std::vector<sAttachment> attachments = {
			{GL_COLOR_ATTACHMENT0, *m_v_blur_buffer.getTexture()}
		};
		m_vBlurFbo.genFbo(attachments);
	}
	{
		std::vector<sAttachment> attachments = {
			{GL_COLOR_ATTACHMENT0, *m_h_blur_buffer.getTexture()}
		};
		m_hBlurFbo.genFbo(attachments);
	}
}

void Blur::run()
{
	m_blurShader.useProgram();

	// Vertical Blur
	{
		m_vBlurFbo.useFbo();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *m_color_buffer);
		glUniform1i(glGetUniformLocation(m_blurShader.getProgram(), "m_tex"), 0);

		glUniform2f(glGetUniformLocation(m_blurShader.getProgram(), "direction"), 0.0f, 1.0f);

		m_mesh->render();
	}

	// Horizontal Blur
	{
		m_hBlurFbo.useFbo();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *m_v_blur_buffer.getTexture());
		glUniform1i(glGetUniformLocation(m_blurShader.getProgram(), "m_tex"), 0);

		glUniform2f(glGetUniformLocation(m_blurShader.getProgram(), "direction"), 1.0f, 0.0f);

		m_mesh->render();
	}
}