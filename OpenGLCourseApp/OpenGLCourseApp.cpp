#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Texture.h"
#include "Light.h"

std::vector<Mesh*> gMeshList;
std::vector<Shader*> gShaderList;
Camera gCamera{ glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.0f}, -90.f, 0.f };
Light gMainLight{ glm::vec3{1.0,1.0,1.0}, 0.2f, glm::vec3{2.0,-1.0,-2.0}, 1.0f };

double gDeltaTime = 0.f;
double gLastCounter = 0.f;
float triOffset = 0.0f;
float triIncrement = 0.002f;

// Vertex Shader
static const char* vShader = "shaders/shader.vert";

// Fragment Shader
static const char* fShader = "shaders/shader.frag";

void CalcAverageNormals(unsigned int* iIndices,
	unsigned int iIndicesCount,
	GLfloat* iVertices,
	unsigned int iVerticesCount,
	unsigned int iLength,
	unsigned int iNormalOffset)
{
	for (size_t i = 0; i < iIndicesCount; i += 3) {
		unsigned int in0 = iIndices[i] * iLength;
		unsigned int in1 = iIndices[i + 1] * iLength;
		unsigned int in2 = iIndices[i + 2] * iLength;

		glm::vec3 v1(iVertices[in1] - iVertices[in0],
			iVertices[in1 + 1] - iVertices[in0 + 1],
			iVertices[in1 + 2] - iVertices[in0 + 2]);

		glm::vec3 v2(iVertices[in2] - iVertices[in0],
			iVertices[in2 + 1] - iVertices[in0 + 1],
			iVertices[in2 + 2] - iVertices[in0 + 2]);

		glm::vec3 wNormal = glm::cross(v1, v2);
		wNormal = glm::normalize(wNormal);

		in0 += iNormalOffset;
		in1 += iNormalOffset;
		in2 += iNormalOffset;
		iVertices[in0] += wNormal.x;
		iVertices[in0 + 1] += wNormal.y;
		iVertices[in0 + 2] += wNormal.z;

		iVertices[in1] += wNormal.x;
		iVertices[in1 + 1] += wNormal.y;
		iVertices[in1 + 2] += wNormal.z;

		iVertices[in2] += wNormal.x;
		iVertices[in2 + 1] += wNormal.y;
		iVertices[in2 + 2] += wNormal.z;
	}

	for(size_t i=0; i < iVerticesCount / iLength; i++){
		unsigned int nOffset = i * iLength + iNormalOffset;
		glm::vec3 vec(iVertices[nOffset], iVertices[nOffset+1], iVertices[nOffset+2]);
		vec = glm::normalize(vec);
		iVertices[nOffset] = vec.x;
		iVertices[nOffset+1] = vec.y;
		iVertices[nOffset+2] = vec.z;
	}
}

void CreateObjects() {
	// x, y, z, u, v, nx, ny, nz
	GLfloat wVertices[] = { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f,0.0f, 0.0f, 0.0f,
		1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,  0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f };

	unsigned int wIndices[] = { 0, 3, 1, 1, 3, 2, 2, 3, 0, 0, 1, 2 };

	CalcAverageNormals(wIndices, 12, wVertices, 32, 8, 5);

	Mesh* wObj = new Mesh();
	wObj->Create(wVertices, wIndices, 32, 12);
	gMeshList.push_back(wObj);
}

void CreateShaders() {
	Shader* sh1 = new Shader();
	sh1->CreateFromFiles(vShader, fShader);
	gShaderList.push_back(sh1);
}

int main() {
	Window wWnd(1920, 1200);
	wWnd.Initialize();

	CreateObjects();
	CreateShaders();

	Texture wBrickTexture("resources/brick.png");
	Texture wDirtTexture("resources/dirt.png");

	glm::mat4 wProjectionMatrix = glm::perspective(
		glm::radians(60.0f),
		(GLfloat)wWnd.GetBufferWidth() / (GLfloat)wWnd.GetBufferHeight(), 0.1f,
		100.f);

	// Loop Until closed
	while (!glfwWindowShouldClose(wWnd.GetGLFWWindow())) {
		GLfloat wCurrentCounter = glfwGetTime();
		gDeltaTime = wCurrentCounter - gLastCounter;
		gLastCounter = wCurrentCounter;

		glfwPollEvents();
		gCamera.Update(wWnd.GetKeysState(), wWnd.GetDeltaX(), wWnd.GetDeltaY(), gDeltaTime);
		triOffset += triIncrement;

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gShaderList[0]->UseShader();

		// Projection and View Matrices
		glUniformMatrix4fv(gShaderList[0]->GetProjectionLocation(), 1, GL_FALSE,
			glm::value_ptr(wProjectionMatrix));
		glUniformMatrix4fv(gShaderList[0]->GetViewLocation(), 1, GL_FALSE,
			glm::value_ptr(gCamera.ComputViewMatrix()));

		// Textures
		glUniform1i(gShaderList[0]->GetDirtTexLocation(), 0);
		wDirtTexture.Use(0);
		glUniform1i(gShaderList[0]->GetBrickTexLocation(), 1);
		wBrickTexture.Use(1);

		// Lights
		gMainLight.UseLight(gShaderList[0]->GetAmbientColourLocation(),
			gShaderList[0]->GetAmbientIntensityLocation(),
			gShaderList[0]->GetDirectionLocation(),
			gShaderList[0]->GetDiffuseIntensityLocation());

		glm::mat4 wModel(1.0f);

		unsigned int i = 0;
		for (auto mesh : gMeshList) {
			wModel = glm::translate(wModel, glm::vec3((float)i * 0.1, (float)i * 0.1,
				(float)i * -2 - 4.0f));

			wModel = glm::rotate(wModel, glm::radians(triOffset * 100),
				glm::vec3(0.f, 1.f, 0.f));

			glUniformMatrix4fv(gShaderList[0]->GetModelLocation(), 1, GL_FALSE,
				glm::value_ptr(wModel));

			mesh->Render();
			i++;
		}
		glUseProgram(0);

		glfwSwapBuffers(wWnd.GetGLFWWindow());
	}
}
