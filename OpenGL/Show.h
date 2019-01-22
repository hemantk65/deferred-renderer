#pragma once

#include "Pass.h"

class Show : public Pass
{
public:
	Show(GLuint *texture, Mesh *mesh);
	~Show();

	void init();
	void run();

	std::vector<GLuint*> getCbuf() { return std::vector<GLuint*> {}; }
	GLuint* getDbuf() { return 0; }

private:
	Shader m_showShader;

	Mesh *m_mesh;
	GLuint *m_texture;
};