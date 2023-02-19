#pragma once

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <string>

#include "CommonValues.h"

class DirectionalLight;
class PointLight;
class SpotLight;


class Shader {
public:
	Shader() {};

	void CreateFromString(const char* iVertexCode, const char* iFragmentCode);
	void CreateFromFiles(const char* iVertexFile, const char* iFragmentFile);

	std::string ReadFile(const char* iFile);

	GLuint GetModelLocation() const { return mUniformModel; }
	GLuint GetViewLocation() const { return mUniformView; }
	GLuint GetProjectionLocation() const { return mUniformProjection; }
	GLuint GetAmbientColorLocation() const { return uniformDirectionalLight.mUniformColor; }
	GLuint GetAmbientIntensityLocation() const { return uniformDirectionalLight.mUniformAmbientIntensity; }
	GLuint GetDirectionLocation() const {return uniformDirectionalLight.mUniformDirection;}
	GLuint GetDiffuseIntensityLocation() const {return uniformDirectionalLight.mUniformDiffuseIntensity;}
	GLuint GetSpecularIntensityLocation() const {return mUniformSpecularIntensity;}
	GLuint GetShininessLocation() const {return mUniformShininess;}
	GLuint GetEyePositionLocation() const {return mUniformEyePosition;}

	GLuint GetTex0Location() const { return mUniformDirtTex; }
	GLuint GetBrickTexLocation() const { return mUniformBrickTex; }

	void SetDirectionalLight(DirectionalLight* iDirLight);
	void SetPointLights(PointLight* iPointLights, GLuint iCount);
	void SetSpotLights(SpotLight* iSpotLights, GLuint iCount);

	void UseShader();
	void ClearShader();

	~Shader();

private:

	void CompileShader(const char* iVertexCode, const char* iFragmentCode);
	void AddShader(GLuint iProgram, const char* iShaderCode, GLenum iShaderType);

	GLuint mPointLightCount = 0;
	GLuint mSpotLightCount = 0;

	struct{
		GLuint mUniformColor;
		GLuint mUniformAmbientIntensity;
		GLuint mUniformDiffuseIntensity;
		GLuint mUniformDirection;
	} uniformDirectionalLight;

	struct {
		GLuint mUniformColor;
		GLuint mUniformAmbientIntensity;
		GLuint mUniformDiffuseIntensity;
		GLuint mUniformPosition;
		GLuint mUniformConstant;
		GLuint mUniformLinear;
		GLuint mUniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	struct {
		GLuint mUniformColor;
		GLuint mUniformAmbientIntensity;
		GLuint mUniformDiffuseIntensity;
		GLuint mUniformPosition;
		GLuint mUniformDirection;
		GLuint mUniformConstant;
		GLuint mUniformLinear;
		GLuint mUniformExponent;
		GLuint mUniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	GLuint mShaderID = 0;
	GLuint mUniformModel = 0;
	GLuint mUniformView = 0;
	GLuint mUniformProjection = 0;
	GLuint mUniformEyePosition = 0;
	GLuint mUniformSpecularIntensity = 0;
	GLuint mUniformShininess = 0;

	GLuint mUniformDirtTex = 0;
	GLuint mUniformBrickTex = 0;

	GLuint mUniformPointLightCount;
	GLuint mUniformSpotLightCount;
};