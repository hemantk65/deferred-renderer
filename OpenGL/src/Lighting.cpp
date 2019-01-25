#include "Lighting.h"

Lighting::Lighting(GLuint *color_buffer, GLuint *normal_buffer, GLuint *position_buffer, GLuint *reflectance_buffer, Mesh *mesh)
	: m_color_buffer(color_buffer), m_normal_buffer(normal_buffer), m_position_buffer(position_buffer), m_reflectance_buffer(reflectance_buffer), m_mesh(mesh)
{
}

Lighting::~Lighting()
{
}

void Lighting::init()
{
	m_lightingShader.loadShaders("lighting.vert", "lighting.frag");

	m_lighting_buffer.genTexture(GL_RGBA8, GL_RGBA, WIDTH, HEIGHT, GL_UNSIGNED_BYTE, nullptr);

	std::vector<sAttachment> attachments = {
		{ GL_COLOR_ATTACHMENT0, *m_lighting_buffer.getTexture() }
	};
	m_lightingFbo.genFbo(attachments);
}

void Lighting::run()
{
	m_lightingShader.useProgram();

	m_lightingFbo.useFbo();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *m_color_buffer);
	glUniform1i(glGetUniformLocation(m_lightingShader.getProgram(), "albedoMap"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *m_normal_buffer);
	glUniform1i(glGetUniformLocation(m_lightingShader.getProgram(), "normalMap"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, *m_position_buffer);
	glUniform1i(glGetUniformLocation(m_lightingShader.getProgram(), "positionMap"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, *m_reflectance_buffer);
	glUniform1i(glGetUniformLocation(m_lightingShader.getProgram(), "reflectanceMap"), 3);

	glUniform3f(glGetUniformLocation(m_lightingShader.getProgram(), "camPos"), 0.0f, 0.0f, 0.0f);

	m_mesh->render();
}