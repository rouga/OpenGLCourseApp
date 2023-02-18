#pragma once
#include "Light.h"

class PointLight : public Light {
public:
	PointLight(){}

	PointLight(glm::vec3 iRGB,
		GLfloat iAmbientIntensity,
		GLfloat iDiffuseIntensity,
		glm::vec3 iPosition,
		GLfloat iConstant,
		GLfloat iLinear,
		GLfloat iExponent);

	void UseLight(GLuint iAmbientColourLocation, GLuint iAmbientIntensityLocation, GLuint iDiffuseIntensityLocation,
		GLuint iPositionLocation, GLfloat iConstantLocation, GLfloat iLinearLocation, GLfloat iExponentLocation);

private:

	glm::vec3 mPosition;
	GLfloat mConstant;
	GLfloat mLinear;
	GLfloat mExponent;
};
