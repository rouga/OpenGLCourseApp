#include <stdio.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

constexpr GLint gWidth = 1280;
constexpr GLint gHeight = 720;

GLuint gVAO, gVBO, gShader;

void CreateTriangle() {
	GLfloat wVertices[] = {
		-1.0f, -1.0f, 0.0,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wVertices), wVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

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
