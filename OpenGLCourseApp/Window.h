#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
 public:
  Window(GLint iWidth, GLint iHeight);
  ~Window();

  int Initialize();

  GLint GetBufferWidth() const { return mWidth; }
  GLint GetBufferHeight() const { return mHeight; }
  GLFWwindow* GetGLFWWindow() const { return mWindow; }

 private:
  GLFWwindow* mWindow;

  GLint mWidth = 1280;
  GLint mHeight = 720;
  GLint mBufferWidth, mBufferHeight;
};