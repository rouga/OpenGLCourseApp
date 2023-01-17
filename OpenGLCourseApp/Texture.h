#pragma once

#include <GL/glew.h>
#include <stb_image.h>

class Texture {
public:
	Texture(const char* iFilepath);
	~Texture();

	void Load();
	void Use(unsigned int iSlot);
	void Clear();

private:
	GLuint mTextureID = 0;
	int mWidth = 0;
	int mHeight = 0;
	int mBitDepth = 0;
	const char* mFilepath;
};