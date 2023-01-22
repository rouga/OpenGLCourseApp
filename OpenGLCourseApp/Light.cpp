#include "Light.h"
#include <glm/gtc/type_ptr.hpp>

Light::Light(glm::vec3 iRGB, GLfloat iAmbientIntensity, glm::vec3 iDirection, GLfloat iDiffuseIntensity)
{
	mColour = iRGB;
	mAmbientIntensity = iAmbientIntensity;
	mDirection = iDirection;
	mDiffuseIntensity = iDiffuseIntensity;
}


void Light::UseLight(GLuint iAmbientColourLocation,
	GLuint iAmbientIntensityLocation,
	GLuint iDirectionLocation,
	GLuint iDiffuseIntensityLocation)
{
	glUniform3f(iAmbientColourLocation, mColour.r, mColour.g, mColour.b);
	glUniform1f(iAmbientIntensityLocation, mAmbientIntensity);
	
	glUniform3fv(iDirectionLocation, 1, glm::value_ptr(mDirection));
	glUniform1f(iDiffuseIntensityLocation, mDiffuseIntensity);
}
