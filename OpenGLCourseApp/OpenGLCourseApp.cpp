#include <stdio.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

constexpr GLint gWidth = 1280;
constexpr GLint gHeight = 720;

GLuint gVAO, gVBO, gProgram;

// Vertex Shader
static const char* vShader = " \n\
#version 460  \n\
layout (location=0) in vec3 pos; \n\
void main() {  \n\
gl_Position = vec4(0.8*pos.x, 0.8*pos.y, pos.z, 1.0); \n\
}";

// Fragment Shader
static const char* fShader = " \n\
#version 460  \n\
out vec4 color; \n\
void main() {  \n\
color = vec4(1.0f, 0.0f, 0.0f, 1.0); \n\
}";

void AddShader(GLuint iProgram, const char* iShaderCode, GLenum iShaderType) {

	GLuint wShader = glCreateShader(iShaderType);

	const GLchar* wCode[1];
	wCode[0] = iShaderCode;

	GLint wCodeLength[1];
	wCodeLength[0] = strlen(iShaderCode);

	glShaderSource(wShader, 1, wCode, wCodeLength);
	glCompileShader(wShader);

	GLint wResult = 0;
	GLchar wLog[512] = { 0 };

	glGetShaderiv(wShader, GL_COMPILE_STATUS, &wResult);
	if (!wResult) {
		glGetShaderInfoLog(gProgram, sizeof(wLog), NULL, wLog);
		std::cerr << "Error Compiling Shader Type  : " << iShaderType << "\n"
			<< wLog;
	}

	glAttachShader(gProgram, wShader);

}

void CompileShaders() {
	gProgram = glCreateProgram();
	if (!gProgram) {
		std::cerr << "Error creating shader program!";
		glfwTerminate();
	}

	AddShader(gProgram, vShader, GL_VERTEX_SHADER);
	AddShader(gProgram, fShader, GL_FRAGMENT_SHADER);

	GLint wResult = 0;
	GLchar wLog[512] = { 0 };

	glLinkProgram(gProgram);
	glGetProgramiv(gProgram, GL_VALIDATE_STATUS, &wResult);
	if (!wResult) {
		glGetProgramInfoLog(gProgram, sizeof(wLog), NULL, wLog);
		std::cerr << "Error Linking program : " << wLog;
	}

	glValidateProgram(gProgram);
	glGetProgramiv(gProgram, GL_LINK_STATUS, &wResult);
	if (!wResult) {
		glGetProgramInfoLog(gProgram, sizeof(wLog), NULL, wLog);
		std::cerr << "Error Validating program : " << wLog;
	}
}

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

// 

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

	CreateTriangle();
	CompileShaders();

	// Loop Until closed
	while (!glfwWindowShouldClose(wMainWindow))
	{
		glfwPollEvents();

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(gProgram);
		glBindVertexArray(gVAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(wMainWindow);
	}
}
