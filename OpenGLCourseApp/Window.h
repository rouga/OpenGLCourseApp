#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

class Window {
public:
	Window(GLint iWidth, GLint iHeight);
	~Window();

	int Initialize();

	GLint GetBufferWidth() const { return mWidth; }
	GLint GetBufferHeight() const { return mHeight; }
	GLFWwindow* GetGLFWWindow() const { return mGLFWWindow; }

	const std::vector<bool>& GetKeysState() const { return mKeysState; }

	GLfloat GetDeltaX();
	GLfloat GetDeltaY();

private:
	void CreateCallbacks();

	static void HandleKeys(GLFWwindow* iGLFWWindow, int iKey, int iCode,
		int iAction, int iMode);
	static void HandleMouse(GLFWwindow* iGLFWWindow, double iPosX, double iPosY);

	GLFWwindow* mGLFWWindow;

	GLint mWidth = 1280;
	GLint mHeight = 720;
	GLint mBufferWidth, mBufferHeight;

	GLfloat mLastX = 0;
	GLfloat mLastY = 0;
	GLfloat mDeltaX = 0;
	GLfloat mDeltaY = 0;
	bool mMouseFirstMovement = true;

	const static int mNumKeys = 512;
	std::vector<bool> mKeysState;
};