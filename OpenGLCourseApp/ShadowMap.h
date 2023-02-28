#pragma once

#include <stdio.h>
#include <GL/glew.h>

class ShadowMap{
public:
	ShadowMap(){}
	~ShadowMap();

	virtual bool Init(GLuint iWidth, GLuint iHeight);
	virtual void Write();
	virtual void Read(GLenum iTextureUnit);

	GLuint GetShadowWidth() const { return mShadowWidth; }
	GLuint GetShadowHeight() const { return mShadowHeight; }

protected:
	GLuint mFBO = 0;
	GLuint mShadowMapTexture = 0;
	GLuint mShadowWidth = 0;
	GLuint mShadowHeight = 0;

};