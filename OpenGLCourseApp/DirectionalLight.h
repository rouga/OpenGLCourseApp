#pragma once

#include "Light.h"

class DirectionalLight : public Light {
public:
	DirectionalLight(){}

	DirectionalLight(glm::vec3 iRGB,
		GLfloat iAmbientIntensity,
		GLfloat iDiffuseIntensity,
		glm::vec3 iDirection, 
		GLfloat iShadowWidth,
		GLfloat iShadowHeight);

	void UseLight(GLuint iAmbientColourLocation, GLuint iAmbientIntensityLocation, GLuint iDiffuseIntensity, GLuint iDirection);
	glm::mat4 CalculateLightTransform();

private:
	glm::vec3 mDirection{ 0.0f, -1.0f, -1.0f };
};