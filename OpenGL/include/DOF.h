#pragma once

#include "Pass.h"

class DOF : public Pass
{
public:
	DOF(GLuint *color_buffer, GLuint *depth_buffer, GLuint *blur_buffer, Mesh *mesh);
	~DOF();

	void init();
	void run();

	std::vector<GLuint*> getCbuf() { return std::vector<GLuint*> { m_dof_buffer.getTexture() }; }
	GLuint* getDbuf() { return 0; }

private:
	Texture m_dof_buffer;
	Framebuffer m_dofFbo;
	Shader m_dofShader;

	Mesh *m_mesh;
	GLuint *m_color_buffer, *m_depth_buffer, *m_blur_buffer;
};