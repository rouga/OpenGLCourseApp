#include "ShadowMap.h"

ShadowMap::~ShadowMap()
{
	if(mFBO){
		glDeleteFramebuffers(1,&mFBO);
	}

	if (mShadowMapTexture) {
		glDeleteTextures(1, &mShadowMapTexture);
	}
}

bool ShadowMap::Init(GLuint iWidth, GLuint iHeight)
{
	mShadowWidth = iWidth;
	mShadowHeight = iHeight;

	glGenTextures(1, &mShadowMapTexture);

	glBindTexture(GL_TEXTURE_2D, mShadowMapTexture);

	glTextureParameteri(mShadowMapTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(mShadowMapTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float wColors[] = {1.0f,1.0f,1.0f,1.0f};
	glTextureParameterfv(mShadowMapTexture, GL_TEXTURE_BORDER_COLOR, wColors);
	glTextureParameteri(mShadowMapTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(mShadowMapTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT,
		mShadowWidth,
		mShadowHeight,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		nullptr);

	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D,
		mShadowMapTexture,
		0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum wStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(wStatus != GL_FRAMEBUFFER_COMPLETE){
		printf("Framebuffer error : %i \n", wStatus);
		return false;
	}

	return true;

}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);	
}

void ShadowMap::Read(GLenum iTextureUnit)
{
	glActiveTexture(iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, mShadowMapTexture);
}

