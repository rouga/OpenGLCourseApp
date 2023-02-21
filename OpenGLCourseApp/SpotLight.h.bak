#pragma once
#include "PointLight.h"

class SpotLight : public PointLight 
{
public:
	SpotLight();


	SpotLight(glm::vec3 iRGB,
		GLfloat iAmbientIntensity,
		GLfloat iDiffuseIntensity,
		glm::vec3 iPosition,
		glm::vec3 iDirection,
		GLfloat iConstant,
		GLfloat iLinear,
		GLfloat iExponent,
		GLfloat iEdge);

	void UseLight(GLuint iAmbientColourLocation, GLuint iAmbientIntensityLocation, GLuint iDiffuseIntensityLocation,
		GLuint iPositionLocation, GLuint iDirectionLocation,
		GLuint iConstantLocation, GLuint iLinearLocation,
		GLuint iExponentLocation, GLuint iEdgeLocation);

private:
	glm::vec3 mDirection = glm::vec3(0.f, -1.f, 0.f);

	GLfloat mEdge = 0.0f;
	GLfloat mProcessedEdge;
};