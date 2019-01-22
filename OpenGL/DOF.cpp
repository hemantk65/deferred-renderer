#include "DOF.h"

DOF::DOF(GLuint *m_color_buffer, GLuint *m_depth_buffer, GLuint *blur_buffer, Mesh *mesh)
	: m_color_buffer(m_color_buffer), m_depth_buffer(m_depth_buffer), m_blur_buffer(blur_buffer), m_mesh(mesh)
{
}

DOF::~DOF()
{
}

void DOF::init()
{
	m_dofShader.loadShaders("dof.vert", "dof.frag");

	m_dof_buffer.genTexture(GL_RGBA8, GL_RGBA, WIDTH, HEIGHT, GL_UNSIGNED_BYTE, nullptr);

	std::vector<sAttachment> attachments = {
		{GL_COLOR_ATTACHMENT0, *m_dof_buffer.getTexture()}
	};
	m_dofFbo.genFbo(attachments);
}

void DOF::run()
{
	m_dofShader.useProgram();

	m_dofFbo.useFbo();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *m_color_buffer);
	glUniform1i(glGetUniformLocation(m_dofShader.getProgram(), "colorTex"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *m_depth_buffer);
	glUniform1i(glGetUniformLocation(m_dofShader.getProgram(), "depthTex"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, *m_blur_buffer);
	glUniform1i(glGetUniformLocation(m_dofShader.getProgram(), "blurTex"), 2);

	m_mesh->render();
}