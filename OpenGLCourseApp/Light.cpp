#include "Light.h"


Light::Light(glm::vec3 iRGB, GLfloat iAmbientIntensity, GLfloat iDiffuseIntensity)
{
	mColour = iRGB;
	mAmbientIntensity = iAmbientIntensity;
	mDiffuseIntensity = iDiffuseIntensity;
}
