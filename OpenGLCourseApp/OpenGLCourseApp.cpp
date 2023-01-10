#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

constexpr GLint gWidth = 1280;
constexpr GLint gHeight = 720;

GLuint gVAO, gVBO, gIBO, gProgram, gUniformModel, gUniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.5;
float triIncrement = 0.002f;

// Vertex Shader
static const char* vShader =
" \n\
#version 460  \n\
uniform mat4 model; \n\
uniform mat4 projection; \n\
layout (location=0) in vec3 pos; \n\
out vec4 v_col; \n\
void main() {  \n\
gl_Position = projection * model * vec4(0.5*pos.x, 0.5*pos.y, pos.z, 1.0); \n\
v_col = vec4(clamp(pos, 0.0f, 1.0f), 1.0f); \n\
}";

// Fragment Shader
static const char* fShader =
" \n\
#version 460  \n\
in vec4 v_col; \n\
out vec4 color; \n\
void main() {  \n\
color = v_col; \n\
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
	glGetProgramiv(gProgram, GL_LINK_STATUS, &wResult);
	if (!wResult) {
		glGetProgramInfoLog(gProgram, sizeof(wLog), NULL, wLog);
		std::cerr << "Error Linking program : " << wLog;
	}

	glValidateProgram(gProgram);
	glGetProgramiv(gProgram, GL_VALIDATE_STATUS, &wResult);
	if (!wResult) {
		glGetProgramInfoLog(gProgram, sizeof(wLog), NULL, wLog);
		std::cerr << "Error Validating program : " << wLog;
	}

	gUniformModel = glGetUniformLocation(gProgram, "model");

	gUniformProjection = glGetUniformLocation(gProgram, "projection");
}

void CreateTriangle() {

	GLfloat wVertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f };

	unsigned int wIndices[] = {
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};

	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wVertices), wVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &gIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wIndices), wIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//

int main() {
	if (!glfwInit()) {
		std::cerr << "GLFW Initialization Failed!";
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* wMainWindow =
		glfwCreateWindow(gWidth, gHeight, "Test Window", NULL, NULL);
	if (!wMainWindow) {
		std::cerr << "GLFW Window Creation Failed !";
		return 1;
	}

	// Get Buffer size information
	int wBufferWidth, wBufferHeight;
	glfwGetFramebufferSize(wMainWindow, &wBufferWidth, &wBufferHeight);

	// Set Context for GLEW to use
	glfwMakeContextCurrent(wMainWindow);

	// Enable Vsync
	glfwSwapInterval(1);
	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW Initialization Failed!";
		glfwDestroyWindow(wMainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport
	glViewport(0, 0, wBufferWidth, wBufferHeight);

	CreateTriangle();
	CompileShaders();

	glm::mat4 wProjectionMatrix = glm::perspective(glm::radians(45.0f),
		(GLfloat)wBufferWidth / (GLfloat)wBufferHeight,
		0.1f,
		100.f);

	// Loop Until closed
	while (!glfwWindowShouldClose(wMainWindow)) {
		glfwPollEvents();
		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}


		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(gProgram);

		glm::mat4 wModel(1.0f);
		wModel = glm::translate(wModel, glm::vec3(0.f, 0.f, -2.f));
		wModel = glm::rotate(wModel, glm::radians(triOffset * 100), glm::vec3(0.f, 1.f, 0.f));

		glUniformMatrix4fv(gUniformModel, 1, GL_FALSE, glm::value_ptr(wModel));
		glUniformMatrix4fv(gUniformProjection, 1, GL_FALSE, glm::value_ptr(wProjectionMatrix));

		glBindVertexArray(gVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(wMainWindow);
	}
}
