#include <iostream>

#include "Renderer.h"

#include <GLFW/glfw3.h>

constexpr auto VSYNC_ENABLE = 1;

void error_callback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Renderer* renderer = (Renderer*)glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
		renderer->updateEye(glm::vec3(0.0f, 0.1f, 0.0f));
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		renderer->updateEye(glm::vec3(0.0f, -0.1f, 0.0f));
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		renderer->updateEye(glm::vec3(-0.1f, 0.0f, 0.0f));
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		renderer->updateEye(glm::vec3(0.1f, 0.0f, 0.0f));
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		renderer->updateCenter(glm::vec3(0.0f, 0.1f, 0.0f));
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		renderer->updateCenter(glm::vec3(0.0f, -0.1f, 0.0f));
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		renderer->updateCenter(glm::vec3(-0.1f, 0.0f, 0.0f));
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		renderer->updateCenter(glm::vec3(0.1f, 0.0f, 0.0f));
}

int main()
{
	Renderer *renderer = new Renderer;

	GLFWwindow* window;

	if (!glfwInit()) return -1;
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Test", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowUserPointer(window, renderer);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(VSYNC_ENABLE);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	renderer->init();

	double previousTime = glfwGetTime();
	int frameCount = 0;

	while (!glfwWindowShouldClose(window))
	{
		renderer->pipe();
		glfwSwapBuffers(window);
		glfwPollEvents();

		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= 1.0)
		{
			std::cout << "FPS: " << frameCount / (currentTime - previousTime) << std::endl;
			frameCount = 0;
			previousTime = currentTime;
		}
	}

	delete renderer;

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}