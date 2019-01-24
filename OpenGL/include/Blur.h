#pragma once

#include "Pass.h"

class Blur : public Pass
{
public:
	Blur(GLuint *color_buffer, Mesh *mesh);
	~Blur();

	void init();
	void run();

	std::vector<GLuint*> getCbuf() { return std::vector<GLuint*> { m_h_blur_buffer.getTexture() }; }
	GLuint* getDbuf() { return 0; }

private:
	Texture m_v_blur_buffer, m_h_blur_buffer;
	Framebuffer m_vBlurFbo, m_hBlurFbo;
	Shader m_blurShader;

	Mesh *m_mesh;
	GLuint *m_color_buffer;
};