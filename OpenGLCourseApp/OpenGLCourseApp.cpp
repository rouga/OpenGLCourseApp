#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

constexpr GLint gWidth = 1280;
constexpr GLint gHeight = 720;

int main()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW Initialization Failed!";
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* wMainWindow = glfwCreateWindow(gWidth, gHeight, "Test Window", NULL, NULL);
	if (!wMainWindow)
	{
		std::cerr << "GLFW Window Creation Failed !";
		return 1;
	}

	// Get Buffer size information
	int wBufferWidth, wBufferHeight;
	glfwGetFramebufferSize(wMainWindow, &wBufferWidth, &wBufferHeight);

	// Set Context for GLEW to use
	glfwMakeContextCurrent(wMainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW Initialization Failed!";
		glfwDestroyWindow(wMainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup Viewport
	glViewport(0, 0, wBufferWidth, wBufferHeight);

	// Loop Until closed
	while (!glfwWindowShouldClose(wMainWindow))
	{
		glfwPollEvents();

		// Clear Window
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(wMainWindow);
	}

	std::cout << "Hello World!\n";
}
