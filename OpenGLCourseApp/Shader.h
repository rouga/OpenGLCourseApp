#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

class Shader {
public:
	Shader() {};

	void CreateFromString(const char* iVertexCode, const char* iFragmentCode);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	void CompileShader(const char* iVertexCode, const char* iFragmentCode);
	void AddShader(GLuint iProgram, const char* iShaderCode, GLenum iShaderType);

	GLuint mShaderID = 0;
	GLuint mUniformProjection = 0;
	GLuint mUniformModel = 0;
};