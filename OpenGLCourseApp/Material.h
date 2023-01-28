#pragma once

#include <gl/glew.h>

class Material{
public:
	Material(GLfloat iSpecularIntensity, GLfloat iShininess);
	void Use(GLuint iSpecularIntensityLocation, GLuint iShininessLocation);
private:
	GLfloat mSpecularIntensity = 0.f;
	GLfloat mShininess = 0.f;
};