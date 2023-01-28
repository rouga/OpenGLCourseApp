#include "Material.h"


Material::Material(GLfloat iIntensity, GLfloat iShininess)
	:mSpecularIntensity(iIntensity),
	mShininess(iShininess)
{

}

void Material::Use(GLuint iSpecularIntensityLocation, GLuint iShininessLocation)
{
	glUniform1f(iSpecularIntensityLocation, mSpecularIntensity);
	glUniform1f(iShininessLocation, mShininess);
}

