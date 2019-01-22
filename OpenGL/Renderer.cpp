#include "Renderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GBuf.h"
#include "Blur.h"
#include "DOF.h"
#include "Show.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

Renderer::Renderer()
{
	m_eye = glm::vec3(0.1f, 0.1f, 0.0f);
	m_center = glm::vec3(-0.2f, -0.2f, -0.5f);

	m_passes.push_back(new GBuf(&m_eye, &m_center, m_tex.getTexture(), &m_sphere));
	m_passes.push_back(new Blur(m_passes[0]->getCbuf()[0], &m_fullScreenQuad));
	m_passes.push_back(new DOF(m_passes[0]->getCbuf()[0], m_passes[0]->getDbuf(), m_passes[1]->getCbuf()[0], &m_fullScreenQuad));
	m_passes.push_back(new Show(m_passes[2]->getCbuf()[0], &m_fullScreenQuad));
}

Renderer::~Renderer()
{
	for (auto pass : m_passes)
		delete pass;
}

void Renderer::initDebug()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback((GLDEBUGPROC)MessageCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, NULL, 1);
}

void Renderer::initModels()
{
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile("suzanne.obj", 0);
		m_sphere.addScene(scene);
	}

	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile("fsq.obj", 0);
		m_fullScreenQuad.addScene(scene);
	}
}

void Renderer::initTextures()
{
	m_tex.loadTexture("uvmap.DDS");
}

void Renderer::init()
{
	initDebug();
	initModels();
	initTextures();

	for (auto pass : m_passes)
		pass->init();
}

void Renderer::pipe()
{
	for (auto pass : m_passes)
		pass->run();
}