#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Camera {
 public:
  Camera(glm::vec3 iPosition, glm::vec3 iWorldUp, GLfloat iStartYaw,
         GLfloat iStartPitch);
  void Update(const std::vector<bool>& iKeyStates, GLfloat iDeltaX, GLfloat iDeltaY, GLfloat iDeltaTime);
  glm::mat4 ComputViewMatrix();

 private:
  // Position vectors
  glm::vec3 mPosition;
  glm::vec3 mFront{0.f, 0.f, -1.0f};
  glm::vec3 mUp;
  glm::vec3 mRight;
  glm::vec3 mWorldUp;

  // Orientation
  GLfloat mYaw;
  GLfloat mPitch;

  // Speed Variables
  // Movement in m/s
  GLfloat mMovementSpeed = 5.f;
  // Arbitrary Rotation factor
  GLfloat mTurnSpeed = 15;

  void KeyControl(const std::vector<bool>& iKeyStates, GLfloat iDeltaTime);
  void MouseControl(GLfloat iDeltaX, GLfloat iDeltaY, GLfloat iDeltaTime);
};