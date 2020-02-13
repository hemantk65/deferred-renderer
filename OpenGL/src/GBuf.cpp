#include "GBuf.h"

GBuf::GBuf(glm::vec3 *m_eye, glm::vec3 *m_center, std::vector<Mesh*> m_meshes)
	: m_eye(m_eye), m_center(m_center), m_meshes(m_meshes)
{
}

GBuf::~GBuf()
{
}

void GBuf::init()
{
	m_gbufShader.loadShaders("gbuf.vert", "gbuf.frag");

	m_color_buffer.genTexture(GL_RGBA8, GL_RGBA, WIDTH, HEIGHT, GL_UNSIGNED_BYTE, nullptr);
	m_normal_buffer.genTexture(GL_RGBA8, GL_RGBA, WIDTH, HEIGHT, GL_UNSIGNED_BYTE, nullptr);
	m_position_buffer.genTexture(GL_RGBA8, GL_RGBA, WIDTH, HEIGHT, GL_UNSIGNED_BYTE, nullptr);
	m_reflectance_buffer.genTexture(GL_RGBA16F, GL_RGBA, WIDTH, HEIGHT, GL_HALF_FLOAT, nullptr);
	m_depth_buffer.genTexture(GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, GL_UNSIGNED_SHORT, nullptr);

	std::vector<sAttachment> attachments = {
		{ GL_COLOR_ATTACHMENT0, *m_color_buffer.getTexture() },
		{ GL_COLOR_ATTACHMENT1, *m_normal_buffer.getTexture() },
		{ GL_COLOR_ATTACHMENT2, *m_position_buffer.getTexture() },
		{ GL_COLOR_ATTACHMENT3, *m_reflectance_buffer.getTexture() },
		{ GL_DEPTH_ATTACHMENT, *m_depth_buffer.getTexture() }
	};
	m_gbufFbo.genFbo(attachments);
}

void GBuf::run()
{
	m_gbufShader.useProgram();

	m_gbufFbo.useFbo();

	glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f));
	glm::mat4 viewMatrix = glm::lookAt(*m_eye, *m_center, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(100.0), 4.0 / 3.0, 0.01, 10.0);

	glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(m_gbufShader.getProgram(), "mvp"), 1, 0, &mvp[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_gbufShader.getProgram(), "model"), 1, 0, &modelMatrix[0][0]);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto mesh : m_meshes)
	{
		mesh->render();
	}

	glDisable(GL_DEPTH_TEST);
}