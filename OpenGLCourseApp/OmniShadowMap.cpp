#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap() : ShadowMap()
{

}

bool OmniShadowMap::Init(GLuint iWidth, GLuint iHeight)
{
	mShadowWidth = iWidth;
	mShadowHeight = iHeight;

	glGenFramebuffers(1, &mFBO);

	glGenTextures(GL_TEXTURE_CUBE_MAP, &mShadowMapTexture);
	for (size_t i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_DEPTH_COMPONENT,
			mShadowWidth,
			mShadowHeight,
			0,
			GL_DEPTH_COMPONENT,
			GL_FLOAT,
			nullptr);
	}

	glTextureParameteri(mShadowMapTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(mShadowMapTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTextureParameteri(mShadowMapTexture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTextureParameteri(mShadowMapTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(mShadowMapTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mShadowMapTexture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum wStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (wStatus != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer error : %i \n", wStatus);
		return false;
	}

	return true;
}
