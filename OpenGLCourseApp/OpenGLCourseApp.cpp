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

std::vector<Mesh*> gMeshList;
std::vector<Shader*> gShaderList;
Camera gCamera{glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.0f}, -90.f, 0.f};

double gDeltaTime = 0.f;
double gLastCounter = 0.f;
float triOffset = 0.0f;
float triIncrement = 0.002f;

// Vertex Shader
static const char* vShader = "shaders/shader.vert";

// Fragment Shader
static const char* fShader = "shaders/shader.frag";

void CreateObjects() {
  GLfloat wVertices[] = {-1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f,
                         1.0f,  -1.0f, 0.0f, 0.0f, 1.0f,  0.0f};

  unsigned int wIndices[] = {0, 3, 1, 1, 3, 2, 2, 3, 0, 0, 1, 2};

  Mesh* wObj = new Mesh();
  wObj->Create(wVertices, wIndices, 12, 12);
  gMeshList.push_back(wObj);
}

void CreateShaders() {
  Shader* sh1 = new Shader();
  sh1->CreateFromFiles(vShader, fShader);
  gShaderList.push_back(sh1);
}

int main() {
  Window wWnd(1600, 1000);
  wWnd.Initialize();

  CreateObjects();
  CreateShaders();

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

    glUniformMatrix4fv(gShaderList[0]->GetProjectionLocation(), 1, GL_FALSE,
                       glm::value_ptr(wProjectionMatrix));
    glUniformMatrix4fv(gShaderList[0]->GetViewLocation(), 1, GL_FALSE,
                       glm::value_ptr(gCamera.ComputViewMatrix()));

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
