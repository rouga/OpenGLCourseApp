#pragma once
#include "ShadowMap.h"

class OmniShadowMap : public ShadowMap {

public:
	OmniShadowMap();
	
	bool Init(GLuint iWidth, GLuint iHeight);
	void Write();
	void Read(GLenum iTextureUnit);

};