#include "Light.h"


Light::Light(glm::vec3 iRGB, GLfloat iAmbientIntensity, GLfloat iDiffuseIntensity,
							GLfloat iShadowWidth, GLfloat iShadowHeight)
{
	mShadowMap = std::make_unique<ShadowMap>();
	mShadowMap->Init(iShadowWidth, iShadowHeight);
	mColour = iRGB;
	mAmbientIntensity = iAmbientIntensity;
	mDiffuseIntensity = iDiffuseIntensity;
}

