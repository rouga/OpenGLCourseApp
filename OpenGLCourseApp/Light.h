#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include "ShadowMap.h"

class Light {
public:
	Light(){}
	Light(glm::vec3 iRGB, GLfloat iAmbientIntensity, GLfloat iDiffuseIntensity, GLfloat iShadowWidth,
		GLfloat iShadowHeight);

	ShadowMap* GetShadowMap() {return mShadowMap.get();}

protected:
	glm::vec3 mColour{ 1.f, 1.f, 1.f };
	GLfloat mAmbientIntensity = 1.f;
	GLfloat mDiffuseIntensity = 1.0f;

	glm::mat4 mLightProj;

	std::unique_ptr<ShadowMap> mShadowMap;
};
