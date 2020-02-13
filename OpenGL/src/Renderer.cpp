#include "Renderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GBuf.h"
#include "Lighting.h"
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
	m_eye = glm::vec3(0.0f, 0.0f, 0.95f);
	m_center = glm::vec3(0.0f, 0.0f, 0.0f);

	std::vector<Mesh*> meshList = { &m_cube, &m_sphere };

	m_passes.push_back(new GBuf(&m_eye, &m_center, meshList));
	m_passes.push_back(new Lighting(m_passes[0]->getCbuf()[0], m_passes[0]->getCbuf()[1], m_passes[0]->getCbuf()[2], m_passes[0]->getCbuf()[3], &m_fullScreenQuad));
	m_passes.push_back(new Blur(m_passes[1]->getCbuf()[0], &m_fullScreenQuad));
	m_passes.push_back(new DOF(m_passes[1]->getCbuf()[0], m_passes[0]->getDbuf(), m_passes[2]->getCbuf()[0], &m_fullScreenQuad));
	m_passes.push_back(new Show(m_passes[3]->getCbuf()[0], &m_fullScreenQuad));
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
		importer.SetPropertyBool(AI_CONFIG_PP_PTV_NORMALIZE, 1);
		const aiScene* scene = importer.ReadFile("resources/nanosuit.obj", aiProcess_Triangulate | aiProcess_RemoveRedundantMaterials | aiProcess_PreTransformVertices | aiProcess_JoinIdenticalVertices);
		m_sphere.addScene(scene);
	}

	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile("resources/fsq.obj", 0);
		m_fullScreenQuad.addScene(scene);
	}

	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile("resources/cube.obj", 0);
		m_cube.addScene(scene);
	}
}

void Renderer::init()
{
	initDebug();
	initModels();

	for (auto pass : m_passes)
		pass->init();
}

void Renderer::pipe()
{
	for (auto pass : m_passes)
		pass->run();
}