#include "Window.h"

#include <iostream>

Window::Window(GLint iWidth, GLint iHeight) {
  mWidth = iWidth;
  mHeight = iHeight;
}

Window::~Window() {
  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

int Window::Initialize() {
  if (!glfwInit()) {
    std::cerr << "GLFW Initialization Failed!";
    glfwTerminate();
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  mWindow = glfwCreateWindow(mWidth, mHeight, "Test Window", NULL, NULL);
  if (!mWindow) {
    std::cerr << "GLFW Window Creation Failed !";
    return 1;
  }

  // Get Buffer size information
  glfwGetFramebufferSize(mWindow, &mBufferWidth, &mBufferHeight);

  // Set Context for GLEW to use
  glfwMakeContextCurrent(mWindow);

  // Enable Vsync
  glfwSwapInterval(1);
  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    std::cerr << "GLEW Initialization Failed!";
    glfwDestroyWindow(mWindow);
    glfwTerminate();
    return 1;
  }

  glEnable(GL_DEPTH_TEST);

  // Setup Viewport
  glViewport(0, 0, mBufferWidth, mBufferHeight);
}
