#include "PointLight.h"
#include <glm/gtc/type_ptr.hpp>

PointLight::PointLight(glm::vec3 iRGB,
	GLfloat iAmbientIntensity,
	GLfloat iDiffuseIntensity,
	glm::vec3 iPosition,
	GLfloat iConstant,
	GLfloat iLinear,
	GLfloat iExponent)

	: Light(iRGB, iAmbientIntensity, iDiffuseIntensity),
	mPosition(iPosition),
	mConstant(iConstant),
	mLinear(iLinear),
	mExponent(iExponent)
{

}

void PointLight::UseLight(GLuint iAmbientColourLocation,
	GLuint iAmbientIntensityLocation,
	GLuint iDiffuseIntensityLocation,
	GLuint iPositionLocation,
	GLfloat iConstantLocation,
	GLfloat iLinearLocation,
	GLfloat iExponentLocation)
{
	glUniform3f(iAmbientColourLocation, mColour.r, mColour.g, mColour.b);
	glUniform1f(iAmbientIntensityLocation, mAmbientIntensity);
	glUniform1f(iDiffuseIntensityLocation, mDiffuseIntensity);
	glUniform3fv(iPositionLocation, 1, glm::value_ptr(mPosition));
	glUniform1f(iConstantLocation, mConstant);
	glUniform1f(iLinearLocation, mLinear);
	glUniform1f(iExponentLocation, mExponent);
}

