#include "DirectionalLight.h"
#include <glm/gtc/type_ptr.hpp>

DirectionalLight::DirectionalLight(glm::vec3 iRGB, GLfloat iAmbientIntensity, GLfloat iDiffuseIntensity, glm::vec3 iDirection)
	: Light(iRGB, iAmbientIntensity, iDiffuseIntensity)
{
	mDirection = iDirection;
}


void DirectionalLight::UseLight(GLuint iAmbientColourLocation,
	GLuint iAmbientIntensityLocation,
	GLuint iDiffuseIntensityLocation,
	GLuint iDirectionLocation)
{
	glUniform3f(iAmbientColourLocation, mColour.r, mColour.g, mColour.b);
	glUniform1f(iAmbientIntensityLocation, mAmbientIntensity);
	glUniform1f(iDiffuseIntensityLocation, mDiffuseIntensity);
	glUniform3fv(iDirectionLocation, 1, glm::value_ptr(mDirection));
}