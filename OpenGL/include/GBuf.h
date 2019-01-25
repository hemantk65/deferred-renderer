#pragma once

#include "Pass.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class GBuf : public Pass
{
public:
	GBuf(glm::vec3 *m_eye, glm::vec3 *m_center, Mesh *m_sphere);
	~GBuf();

	void init();
	void run();

	std::vector<GLuint*> getCbuf() { return std::vector<GLuint*> { m_color_buffer.getTexture(), m_normal_buffer.getTexture(), m_position_buffer.getTexture(), m_reflectance_buffer.getTexture() }; }
	GLuint* getDbuf() { return m_depth_buffer.getTexture(); }

private:
	Texture m_color_buffer, m_normal_buffer, m_position_buffer, m_reflectance_buffer, m_depth_buffer;
	Framebuffer m_gbufFbo;
	Shader m_gbufShader;

	glm::vec3 *m_eye, *m_center;
	Mesh *m_sphere;
};