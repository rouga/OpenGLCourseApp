#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Mesh.h"
#include "Shader.h"

constexpr GLint gWidth = 1280;
constexpr GLint gHeight = 720;

std::vector<Mesh*> gMeshList;
std::vector<Shader*> gShaderList;

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
gl_Position = projection * model * vec4(pos.x, pos.y, pos.z, 1.0); \n\
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

void CreateObjects() {
	GLfloat wVertices[] = { -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f,
						   1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  0.0f };

	unsigned int wIndices[] = { 0, 3, 1, 1, 3, 2, 2, 3, 0, 0, 1, 2 };

	Mesh* wObj = new Mesh();
	wObj->Create(wVertices, wIndices, 12, 12);
	gMeshList.push_back(wObj);
}

void CreateShaders() {
	Shader* sh1 = new Shader();
	sh1->CreateFromString(vShader, fShader);
	gShaderList.push_back(sh1);
}

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

	CreateObjects();
	CreateShaders();

	glm::mat4 wProjectionMatrix = glm::perspective(
		glm::radians(60.0f), (GLfloat)wBufferWidth / (GLfloat)wBufferHeight, 0.1f,
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

		gShaderList[0]->UseShader();

		glUniformMatrix4fv(gShaderList[0]->GetProjectionLocation(), 1, GL_FALSE,
			glm::value_ptr(wProjectionMatrix));

		glm::mat4 wModel(1.0f);

		unsigned int i = 0;
		for (auto mesh : gMeshList) {
			wModel = glm::translate(wModel, glm::vec3((float)i * 0.1, (float)i * 0.1,
				(float)i * -2 - 4.0f));
			wModel = glm::rotate(wModel, glm::radians(triOffset * 100),
				glm::vec3(0.f, 1.f, 0.f));
			glUniformMatrix4fv(gShaderList[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(wModel));

			mesh->Render();
			i++;
		}
		glUseProgram(0);

		glfwSwapBuffers(wMainWindow);
	}
}
