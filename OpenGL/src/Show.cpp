#include "Show.h"

Show::Show(GLuint *texture, Mesh *mesh)
	: m_texture(texture), m_mesh(mesh)
{
}

Show::~Show()
{
}

void Show::init()
{
	m_showShader.loadShaders("show.vert", "show.frag");
}

void Show::run()
{
	Framebuffer defaultFbo;
	defaultFbo.useFbo();

	m_showShader.useProgram();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *m_texture);
	glUniform1i(glGetUniformLocation(m_showShader.getProgram(), "m_tex"), 0);

	m_mesh->render();
}