#pragma once

#include "Pass.h"

class Lighting : public Pass
{
public:
	Lighting(GLuint *color_buffer, GLuint *normal_buffer, GLuint *position_buffer, GLuint *reflectance_buffer, Mesh *mesh);
	~Lighting();

	void init();
	void run();

	std::vector<GLuint*> getCbuf() { return std::vector<GLuint*> { m_lighting_buffer.getTexture() }; }
	GLuint* getDbuf() { return 0; }

private:
	Texture m_lighting_buffer;
	Framebuffer m_lightingFbo;
	Shader m_lightingShader;

	Mesh *m_mesh;
	GLuint *m_color_buffer, *m_normal_buffer, *m_position_buffer, *m_reflectance_buffer;
};