#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>


class Light {
public:
	Light() {};
	Light(GLfloat iRed, GLfloat iGreen, GLfloat iBlue, GLfloat iIntensity);

	void UseLight(GLuint iAmbientColourLocation, GLuint iAmbientIntensityLocation);

private:
	glm::vec3 mColour{ 1.f, 1.f, 1.f };
	GLfloat mAmbientIntensity = 1.f;
};
