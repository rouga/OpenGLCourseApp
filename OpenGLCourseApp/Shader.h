#pragma once

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <string>

class Shader {
public:
	Shader() {};

	void CreateFromString(const char* iVertexCode, const char* iFragmentCode);
	void CreateFromFiles(const char* iVertexFile, const char* iFragmentFile);

	std::string ReadFile(const char* iFile);

	GLuint GetModelLocation() const { return mUniformModel; }
	GLuint GetViewLocation() const { return mUniformView; }
	GLuint GetProjectionLocation() const { return mUniformProjection; }
	GLuint GetAmbientColourLocation() const { return mUniformAmbientColour; }
	GLuint GetAmbientIntensityLocation() const { return mUniformAmbientIntensity; }
	GLuint GetDirectionLocation() const {return mUniformDirection;}
	GLuint GetDiffuseIntensityLocation() const {return mUniformDiffuseIntensity;}
	GLuint GetSpecularIntensityLocation() const {return mUniformSpecularIntensity;}
	GLuint GetShininessLocation() const {return mUniformShininess;}
	GLuint GetEyePositionLocation() const {return mUniformEyePosition;}

	GLuint GetDirtTexLocation() const { return mUniformDirtTex; }
	GLuint GetBrickTexLocation() const { return mUniformBrickTex; }

	void UseShader();
	void ClearShader();

	~Shader();

private:
	void CompileShader(const char* iVertexCode, const char* iFragmentCode);
	void AddShader(GLuint iProgram, const char* iShaderCode, GLenum iShaderType);

	GLuint mShaderID = 0;
	GLuint mUniformModel = 0;
	GLuint mUniformView = 0;
	GLuint mUniformProjection = 0;
	GLuint mUniformAmbientColour = 0;
	GLuint mUniformAmbientIntensity = 0;
	GLuint mUniformDirection = 0;
	GLuint mUniformDiffuseIntensity = 0;
	GLuint mUniformEyePosition = 0;
	GLuint mUniformSpecularIntensity = 0;
	GLuint mUniformShininess = 0;


	GLuint mUniformDirtTex = 0;
	GLuint mUniformBrickTex = 0;
};