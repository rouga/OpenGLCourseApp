#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>


class Light {
public:
	Light() {};
	Light(glm::vec3 iRGB, GLfloat iAmbientIntensity, glm::vec3 iDirection, GLfloat iDiffuseIntensity);

	void UseLight(GLuint iAmbientColourLocation, GLuint iAmbientIntensityLocation, GLuint iDirection, GLuint iDiffuseIntensity);

private:
	glm::vec3 mColour{ 1.f, 1.f, 1.f };
	GLfloat mAmbientIntensity = 1.f;

	glm::vec3 mDirection{ 0.0f, -1.0f, -1.0f };
	GLfloat mDiffuseIntensity = 1.0f;
};
