#include "Shader.h"

void Shader::CreateFromString(const char* iVertexCode,
	const char* iFragmentCode) {
	CompileShader(iVertexCode, iFragmentCode);
}

void Shader::CreateFromFiles(const char* iVertexFile,
	const char* iFragmentFile) {
	std::string wVertexString = ReadFile(iVertexFile);
	std::string wFragmentString = ReadFile(iFragmentFile);

	const char* wVertexCode = wVertexString.c_str();
	const char* wFragmentCode = wFragmentString.c_str();

	CompileShader(wVertexCode, wFragmentCode);
}

std::string Shader::ReadFile(const char* iFile) {
	// Read File
	std::ifstream wFilestream(iFile, std::ios::in);

	if (!wFilestream.is_open()) {
		std::cerr << "Failed to read " << iFile << " : File Doesn't exist !";
		return "";
	}

	// Permanent and temporary containers for shader code
	std::string wContent;
	std::string wLine = "";

	while (!wFilestream.eof()) {
		std::getline(wFilestream, wLine);
		wContent.append(wLine + "\n");
	}

	wFilestream.close();
	return wContent;
}

GLuint Shader::GetProjectionLocation() { return mUniformProjection; }

GLuint Shader::GetModelLocation() { return mUniformModel; }

GLuint Shader::GetViewLocation() { return mUniformView; }

void Shader::UseShader() { glUseProgram(mShaderID); }

void Shader::ClearShader() {
	if (mShaderID) {
		glDeleteProgram(mShaderID);
		mShaderID = 0;
	}

	mUniformModel = 0;
	mUniformView = 0;
	mUniformProjection = 0;
}

Shader::~Shader() {}

void Shader::CompileShader(const char* iVertexCode, const char* iFragmentCode) {
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
	mUniformView = glGetUniformLocation(mShaderID, "view");
	mUniformProjection = glGetUniformLocation(mShaderID, "projection");

	mUniformDirtTex = glGetUniformLocation(mShaderID, "dirtTexture");
	mUniformBrickTex = glGetUniformLocation(mShaderID, "brickTexture");

}

void Shader::AddShader(GLuint iProgram, const char* iShaderCode,
	GLenum iShaderType) {
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
