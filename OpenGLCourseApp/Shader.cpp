#include "Shader.h"

void Shader::CreateFromString(const char* iVertexCode, const char* iFragmentCode)
{
	CompileShader(iVertexCode, iFragmentCode);
}

GLuint Shader::GetProjectionLocation()
{
	return mUniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return mUniformModel;
}

void Shader::UseShader()
{
	glUseProgram(mShaderID);
}

void Shader::ClearShader()
{
	if (mShaderID) {
		glDeleteProgram(mShaderID);
		mShaderID = 0;
	}

	mUniformModel = 0;
	mUniformProjection = 0;
}

Shader::~Shader()
{
}

void Shader::CompileShader(const char* iVertexCode, const char* iFragmentCode)
{
	mShaderID = glCreateProgram();
	if (!mShaderID) {
		std::cerr << "Error creating shader program!";
		return;
	}

	AddShader(mShaderID, iVertexCode, GL_VERTEX_SHADER);
	AddShader(mShaderID, iFragmentCode, GL_FRAGMENT_SHADER);

	GLint wResult = 0;
	GLchar wLog[512] = { 0 };

	glLinkProgram(mShaderID);
	glGetProgramiv(mShaderID, GL_LINK_STATUS, &wResult);
	if (!wResult) {
		glGetProgramInfoLog(mShaderID, sizeof(wLog), NULL, wLog);
		std::cerr << "Error Linking program : " << wLog;
	}

	glValidateProgram(mShaderID);
	glGetProgramiv(mShaderID, GL_VALIDATE_STATUS, &wResult);
	if (!wResult) {
		glGetProgramInfoLog(mShaderID, sizeof(wLog), NULL, wLog);
		std::cerr << "Error Validating program : " << wLog;
	}

	mUniformModel = glGetUniformLocation(mShaderID, "model");

	mUniformProjection = glGetUniformLocation(mShaderID, "projection");
}

void Shader::AddShader(GLuint iProgram, const char* iShaderCode, GLenum iShaderType)
{
	GLuint wShader = glCreateShader(iShaderType);

	const GLchar* wCode[1];
	wCode[0] = iShaderCode;

	GLint wCodeLength[1];
	wCodeLength[0] = strlen(iShaderCode);

	glShaderSource(wShader, 1, wCode, wCodeLength);
	glCompileShader(wShader);

	GLint wResult = 0;
	GLchar wLog[512] = { 0 };

	glGetShaderiv(wShader, GL_COMPILE_STATUS, &wResult);
	if (!wResult) {
		glGetShaderInfoLog(iProgram, sizeof(wLog), NULL, wLog);
		std::cerr << "Error Compiling Shader Type  : " << iShaderType << "\n"
			<< wLog;
	}

	glAttachShader(iProgram, wShader);
}
