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

	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetProjectionLocation();

	GLuint GetDirtTexLocation() const { return mUniformDirtTex; }
	GLuint GetBrickTexLocation() const {return mUniformBrickTex;}

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

	GLuint mUniformDirtTex = 0;
	GLuint mUniformBrickTex = 0;
};