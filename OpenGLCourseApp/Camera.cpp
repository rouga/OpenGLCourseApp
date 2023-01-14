#include "Camera.h"

#include <GLFW/glfw3.h>

#include "Window.h"

Camera::Camera(glm::vec3 iPosition, glm::vec3 iWorldUp, GLfloat iStartYaw,
               GLfloat iStartPitch)
    : mPosition(iPosition),
      mWorldUp(iWorldUp),
      mYaw(iStartYaw),
      mPitch(iStartPitch) {
  void Update();
}

void Camera::KeyControl(const std::vector<bool>& iKeyStates,
                        GLfloat iDeltaTime) {
  GLfloat wVelocity = mMovementSpeed * iDeltaTime;
  if (iKeyStates[GLFW_KEY_W]) {
    mPosition += mFront * wVelocity;
  }

  if (iKeyStates[GLFW_KEY_S]) {
    mPosition -= mFront * wVelocity;
  }

  if (iKeyStates[GLFW_KEY_D]) {
    mPosition += mRight * wVelocity;
  }

  if (iKeyStates[GLFW_KEY_A]) {
    mPosition -= mRight * wVelocity;
  }

  // Up and Down in WorldSpace
  if (iKeyStates[GLFW_KEY_E]) {
    mPosition += mWorldUp * wVelocity;
  }
  if (iKeyStates[GLFW_KEY_Q]) {
    mPosition -= mWorldUp * wVelocity;
  }
}

void Camera::MouseControl(GLfloat iDeltaX, GLfloat iDeltaY,
                          GLfloat iDeltaTime) {
  iDeltaX *= mTurnSpeed * iDeltaTime;
  iDeltaY *= mTurnSpeed * iDeltaTime;

  mYaw += iDeltaX;
  mPitch += iDeltaY;

  if (abs(mPitch) > 89.f) {
    mPitch = mPitch > 0 ? 89.f : -89.f;
  }
}

glm::mat4 Camera::ComputViewMatrix() {
  return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::Update(const std::vector<bool>& iKeyStates, GLfloat iDeltaX,
                    GLfloat iDeltaY, GLfloat iDeltaTime) {
  MouseControl(iDeltaX, iDeltaY, iDeltaTime);
  // Update Front
  mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
  mFront.y = sin(glm::radians(mPitch));
  mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
  mFront = glm::normalize(mFront);

  // Update Right
  mRight = glm::normalize(glm::cross(mFront, mWorldUp));
  mUp = glm::normalize(glm::cross(mRight, mFront));

  KeyControl(iKeyStates, iDeltaTime);
}