#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>

#include "Mesh.h"
#include "Texture.h"
#include "Pass.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void init();
	void pipe();

	void updateEye(glm::vec3 translation)
	{
		m_eye += translation;
		std::cout << "Eye: " << glm::to_string(m_eye) << std::endl;
	}

	void updateCenter(glm::vec3 translation)
	{
		m_center += translation;
		std::cout << "Center: " << glm::to_string(m_center) << std::endl;
	}

private:
	void initDebug();
	void initModels();
	void initTextures();

	std::vector<Pass*> m_passes;

	glm::vec3 m_eye;
	glm::vec3 m_center;

	Mesh m_sphere, m_fullScreenQuad;

	Texture m_tex;
};