#include "Light.h"

Light::Light(GLfloat iRed, GLfloat iGreen, GLfloat iBlue, GLfloat iIntensity)
{
	mColour = glm::vec3{ iRed, iGreen, iBlue };
	mAmbientIntensity = iIntensity;
}

void Light::UseLight(GLuint iAmbientColourLocation, GLuint iAmbientIntensityLocation)
{
	glUniform3f(iAmbientColourLocation, mColour.r, mColour.g, mColour.b);
	glUniform1f(iAmbientIntensityLocation, mAmbientIntensity);
}	