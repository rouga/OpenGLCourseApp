#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>


class Light {
public:
	Light() {};
	Light(glm::vec3 iRGB, GLfloat iAmbientIntensity, GLfloat iDiffuseIntensity);

protected:
	glm::vec3 mColour{ 1.f, 1.f, 1.f };
	GLfloat mAmbientIntensity = 1.f;
	GLfloat mDiffuseIntensity = 1.0f;
};
