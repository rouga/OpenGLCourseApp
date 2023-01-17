#include "Window.h"

#include <iostream>

Window::Window(GLint iWidth, GLint iHeight) {
	mWidth = iWidth;
	mHeight = iHeight;
	mKeysState.reserve(mNumKeys);
	for (size_t i = 0; i < mNumKeys; i++) {
		mKeysState.push_back(false);
	}
}

Window::~Window() {
	glfwDestroyWindow(mGLFWWindow);
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

	mGLFWWindow = glfwCreateWindow(mWidth, mHeight, "Test Window", NULL, NULL);
	if (!mGLFWWindow) {
		std::cerr << "GLFW Window Creation Failed !";
		return 1;
	}

	// Get Buffer size information
	glfwGetFramebufferSize(mGLFWWindow, &mBufferWidth, &mBufferHeight);

	// Set Context for GLEW to use
	glfwMakeContextCurrent(mGLFWWindow);

	// Set callbacks
	CreateCallbacks();
	glfwSetInputMode(mGLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Enable Vsync
	glfwSwapInterval(1);
	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW Initialization Failed!";
		glfwDestroyWindow(mGLFWWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport
	glViewport(0, 0, mBufferWidth, mBufferHeight);
	glfwSetWindowUserPointer(mGLFWWindow, this);
}

GLfloat Window::GetDeltaX() {
	GLfloat wDeltaX = mDeltaX;
	mDeltaX = 0;
	return wDeltaX;
}

GLfloat Window::GetDeltaY() {
	GLfloat wDeltaY = mDeltaY;
	mDeltaY = 0;
	return wDeltaY;
}

void Window::CreateCallbacks() {
	glfwSetKeyCallback(mGLFWWindow, HandleKeys);
	glfwSetCursorPosCallback(mGLFWWindow, HandleMouse);
}

void Window::HandleKeys(GLFWwindow* iGLFWWindow, int iKey, int iCode,
	int iAction, int iMode) {
	Window* wWindow = static_cast<Window*>(glfwGetWindowUserPointer(iGLFWWindow));

	if (iKey == GLFW_KEY_ESCAPE && iAction == GLFW_PRESS) {
		glfwSetWindowShouldClose(iGLFWWindow, GLFW_TRUE);
	}

	if (iKey >= 0 && iKey < wWindow->mNumKeys) {
		if (iAction == GLFW_PRESS) {
			wWindow->mKeysState[iKey] = true;

		}
		else if (iAction == GLFW_RELEASE) {
			wWindow->mKeysState[iKey] = false;
		}
	}
}

void Window::HandleMouse(GLFWwindow* iGLFWWindow, double iPosX, double iPosY) {
	Window* wWindow = static_cast<Window*>(glfwGetWindowUserPointer(iGLFWWindow));

	if (wWindow->mMouseFirstMovement) {
		wWindow->mLastX = iPosX;
		wWindow->mLastY = iPosY;
		wWindow->mMouseFirstMovement = false;
	}

	wWindow->mDeltaX = iPosX - wWindow->mLastX;
	wWindow->mDeltaY = wWindow->mLastY - iPosY;

	wWindow->mLastX = iPosX;
	wWindow->mLastY = iPosY;
}
