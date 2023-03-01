#include "DirectionalLight.h"
#include <glm/gtc/type_ptr.hpp>

DirectionalLight::DirectionalLight(glm::vec3 iRGB, GLfloat iAmbientIntensity, GLfloat iDiffuseIntensity, glm::vec3 iDirection,
	GLfloat iShadowWidth, GLfloat iShadowHeight)
	: Light(iRGB, iAmbientIntensity, iDiffuseIntensity, iShadowWidth, iShadowHeight)
{
	mDirection = iDirection;
	mLightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
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

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return mLightProj * glm::lookAt(-mDirection, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
