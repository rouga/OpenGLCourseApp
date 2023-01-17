#include <iostream>

#include "Texture.h"


Texture::Texture(const char* iFilepath) : mFilepath(iFilepath) {
	Load();
}

Texture::~Texture() {
	Clear();
}

void Texture::Load() {

	unsigned char* wTexData = stbi_load(mFilepath,
		&mWidth,
		&mHeight,
		&mBitDepth,
		0);

	stbi_set_flip_vertically_on_load(1);
	if (!wTexData) {
		std::cerr << "Failed to find " << mFilepath << "\n";
		return;
	}


	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(mTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(mTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		mWidth,
		mHeight,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		wTexData);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(wTexData);
}

void Texture::Use(unsigned int iSlot)
{
	glBindTextureUnit(iSlot, mTextureID);
}

void Texture::Clear()
{
	glDeleteTextures(1, &mTextureID);
	mTextureID = 0;
	mWidth = 0;
	mHeight = 0;
	mBitDepth = 0;
	mFilepath = nullptr;
}
