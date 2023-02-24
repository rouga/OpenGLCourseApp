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

#include "CommonValues.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

std::vector<Mesh*> gMeshList;
std::vector<Shader*> gShaderList;

Camera gCamera{ glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.0f}, -90.f, 0.f };
DirectionalLight gDirectionalLight{ glm::vec3{1.0,1.0,1.0}, 0.2f,1.0f, glm::vec3{2.0,-1.0,-2.0} };
PointLight gPointLights[MAX_POINT_LIGHTS];
SpotLight gSpotLights[MAX_SPOT_LIGHTS];

Material gShinyMaterial(1.0f, 32.f);
Material gDullMaterial(0.3f, 4.f);

Model gAirplane("Airplane");

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

	for (size_t i = 0; i < iVerticesCount / iLength; i++) {
		unsigned int nOffset = i * iLength + iNormalOffset;
		glm::vec3 vec(iVertices[nOffset], iVertices[nOffset + 1], iVertices[nOffset + 2]);
		vec = glm::normalize(vec);
		iVertices[nOffset] = vec.x;
		iVertices[nOffset + 1] = vec.y;
		iVertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects() {
	// x, y, z, u, v, nx, ny, nz
	GLfloat wPyramidVertices[] = { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f,0.0f, 0.0f, 0.0f,
		1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,  0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f };

	unsigned int wPyramidIndices[] = { 0, 3, 1, 1, 3, 2, 2, 3, 0, 0, 1, 2 };

	CalcAverageNormals(wPyramidIndices, 12, wPyramidVertices, 32, 8, 5);

	GLfloat wFloorVertices[] = { -10.f, 0.0f, -10.f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	10.0f, 0.0f, -10.f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	-10.f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
	10.f, 0.0f, 10.f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f };

	GLuint wFloorIndices[] = {
	0, 2, 1,
	1, 2, 3,
	};



	Mesh* wPyramidMesh = new Mesh();
	wPyramidMesh->Create(wPyramidVertices, wPyramidIndices, 32, 12);
	gMeshList.push_back(wPyramidMesh);

	Mesh* wFloorMesh = new Mesh();
	wFloorMesh->Create(wFloorVertices, wFloorIndices, 32, 6);
	gMeshList.push_back(wFloorMesh);
}

void CreateShaders() {
	Shader* sh1 = new Shader();
	sh1->CreateFromFiles(vShader, fShader);
	gShaderList.push_back(sh1);
}

int main() {
	Window wWnd(1920, 1200);
	wWnd.Initialize();

	GLuint wPointLightCount = 0;
	GLuint wSpotLightCount = 0;

	gPointLights[0] = PointLight(glm::vec3(0.f, 1.0f, 0.0f), 0.1f, 1.0f, glm::vec3(-4.f, 2.f, 0.f), 0.3f, 0.1f, 0.1f);
	wPointLightCount++;
	gPointLights[1] = PointLight(glm::vec3(0.f, 0.0f, 1.0f), 0.1f, 0.4f, glm::vec3(4.f, 0.f, 0.f), 0.3f, 0.2f, 0.1f);
	wPointLightCount++;

	gSpotLights[0] = SpotLight(glm::vec3(1.f, 1.f, 1.f), 0.0f, 1.0f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f), 0.3f, 0.2f, 0.1f, 40.0f);
	wSpotLightCount++;

	CreateObjects();
	CreateShaders();

	Texture wBrickTexture("resources/Textures/brick.png", true);
	Texture wDirtTexture("resources/Textures/dirt.png", true);
	Texture wPlainTexture("resources/Textures/plain.png", true);

	wBrickTexture.Load();
	wDirtTexture.Load();
	wPlainTexture.Load();

	gAirplane.Load("resources/Models/Airplane/11803_Airplane_v1_l1.obj");

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
		glUniform3f(gShaderList[0]->GetEyePositionLocation(),
			gCamera.GetPosition().r,
			gCamera.GetPosition().g,
			gCamera.GetPosition().b);

		//gSpotLights[0].SetFlash(gCamera.GetPosition(), gCamera.GetDirection());

		// Lights
		gShaderList[0]->SetDirectionalLight(&gDirectionalLight);
		gShaderList[0]->SetPointLights(&gPointLights[0], wPointLightCount);
		//gShaderList[0]->SetSpotLights(gSpotLights, wSpotLightCount);

		glm::mat4 wModel(1.0f);

		unsigned int i = 0;

		wModel = glm::translate(wModel, glm::vec3(0.0f, 0.0f, -2.5f));
		wModel = glm::rotate(wModel, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
		wModel = glm::rotate(wModel, glm::radians(triOffset * 100),
			glm::vec3(0.f, 0.f, 1.f));

		wModel = glm::scale(wModel, glm::vec3(0.001, 0.001, 0.001));

		glUniformMatrix4fv(gShaderList[0]->GetModelLocation(), 1, GL_FALSE,
			glm::value_ptr(wModel));

		// Textures
		glUniform1i(gShaderList[0]->GetTex0Location(), 0);

		gShinyMaterial.Use(gShaderList[0]->GetSpecularIntensityLocation(),
			gShaderList[0]->GetShininessLocation());
		gAirplane.Render();

		wModel = glm::mat4(1.0f);
		wModel = glm::translate(wModel, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(gShaderList[0]->GetModelLocation(), 1, GL_FALSE,
			glm::value_ptr(wModel));

		wDirtTexture.Use(0);
		gDullMaterial.Use(gShaderList[0]->GetSpecularIntensityLocation(),
			gShaderList[0]->GetShininessLocation());

		gMeshList[1]->Render();

		glUseProgram(0);

		glfwSwapBuffers(wWnd.GetGLFWWindow());
	}
}
