#pragma once

#include <GL/glew.h>

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Framebuffer.h"

#define WIDTH 640
#define HEIGHT 480

class Pass
{
public:
	virtual ~Pass() {};

	virtual void init() = 0;
	virtual void run() = 0;

	virtual std::vector<GLuint*> getCbuf() = 0;
	virtual GLuint* getDbuf() = 0;
};