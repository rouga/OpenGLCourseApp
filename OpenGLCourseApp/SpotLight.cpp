#include "SpotLight.h"
#include <glm/gtc/type_ptr.hpp>

SpotLight::SpotLight(glm::vec3 iRGB,
	GLfloat iAmbientIntensity,
	GLfloat iDiffuseIntensity,
	glm::vec3 iPosition,
	glm::vec3 iDirection,
	GLfloat iConstant,
	GLfloat iLinear,
	GLfloat iExponent,
	GLfloat iEdge)
	: PointLight(iRGB, iAmbientIntensity, iDiffuseIntensity, iPosition, iConstant, iLinear, iExponent),
	mEdge(iEdge)
{
	mDirection = glm::normalize(iDirection);
	mProcessedEdge = cosf(glm::radians(mEdge));
}

void SpotLight::UseLight(GLuint iAmbientColourLocation,
	GLuint iAmbientIntensityLocation,
	GLuint iDiffuseIntensityLocation,
	GLuint iPositionLocation,
	GLuint iDirectionLocation,
	GLuint iConstantLocation,
	GLuint iLinearLocation,
	GLuint iExponentLocation,
	GLuint iEdgeLocation)
{

	glUniform3f(iAmbientColourLocation, mColour.r, mColour.g, mColour.b);
	glUniform1f(iAmbientIntensityLocation, mAmbientIntensity);
	glUniform1f(iDiffuseIntensityLocation, mDiffuseIntensity);
	glUniform3fv(iPositionLocation, 1, glm::value_ptr(mPosition));
	glUniform3fv(iDirectionLocation, 1, glm::value_ptr(mDirection));
	glUniform1f(iConstantLocation, mConstant);
	glUniform1f(iLinearLocation, mLinear);
	glUniform1f(iExponentLocation, mExponent);
	glUniform1f(iEdgeLocation, mProcessedEdge);
}

void SpotLight::SetFlash(glm::vec3 iPosition, glm::vec3 iDirection)
{
	mPosition = iPosition;
	mDirection = iDirection;
}
