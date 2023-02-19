#include "Shader.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

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

void Shader::SetDirectionalLight(DirectionalLight* iDirLight)
{
	iDirLight->UseLight(uniformDirectionalLight.mUniformColor,
		uniformDirectionalLight.mUniformAmbientIntensity,
		uniformDirectionalLight.mUniformDiffuseIntensity,
		uniformDirectionalLight.mUniformDirection);
}

void Shader::SetPointLights(PointLight* iPointLights, GLuint iCount)
{
	if (iCount > MAX_POINT_LIGHTS) {
		iCount = MAX_POINT_LIGHTS;
	}

	glUniform1i(mUniformPointLightCount, iCount);

	for (size_t i = 0; i < iCount; i++) {

		iPointLights[i].UseLight(uniformPointLight[i].mUniformColor,
			uniformPointLight[i].mUniformAmbientIntensity,
			uniformPointLight[i].mUniformDiffuseIntensity,
			uniformPointLight[i].mUniformPosition,
			uniformPointLight[i].mUniformConstant,
			uniformPointLight[i].mUniformLinear,
			uniformPointLight[i].mUniformExponent);
	}
}

void Shader::SetSpotLights(SpotLight* iSpotLights, GLuint iCount) {
	if (iCount > MAX_SPOT_LIGHTS) {
		iCount = MAX_SPOT_LIGHTS;
	}

	glUniform1i(mUniformSpotLightCount, iCount);

	for (size_t i = 0; i < iCount; i++) {

		iSpotLights[i].UseLight(uniformSpotLight[i].mUniformColor,
			uniformSpotLight[i].mUniformAmbientIntensity,
			uniformSpotLight[i].mUniformDiffuseIntensity,
			uniformSpotLight[i].mUniformPosition,
			uniformSpotLight[i].mUniformDirection,
			uniformSpotLight[i].mUniformConstant,
			uniformSpotLight[i].mUniformLinear,
			uniformSpotLight[i].mUniformExponent,
			uniformSpotLight[i].mUniformEdge);
	}
}

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

	mUniformDirtTex = glGetUniformLocation(mShaderID, "u_TextureSlot0");
	mUniformBrickTex = glGetUniformLocation(mShaderID, "brickTexture");

	uniformDirectionalLight.mUniformColor = glGetUniformLocation(mShaderID, "u_directionalLight.base.colour");
	uniformDirectionalLight.mUniformAmbientIntensity = glGetUniformLocation(mShaderID, "u_directionalLight.base.ambientIntensity");

	uniformDirectionalLight.mUniformDirection = glGetUniformLocation(mShaderID, "u_directionalLight.direction");
	uniformDirectionalLight.mUniformDiffuseIntensity = glGetUniformLocation(mShaderID, "u_directionalLight.base.diffuseIntensity");

	mUniformSpecularIntensity = glGetUniformLocation(mShaderID, "u_material.specularIntensity");
	mUniformShininess = glGetUniformLocation(mShaderID, "u_material.shininess");

	mUniformEyePosition = glGetUniformLocation(mShaderID, "u_EyePosition");

	mUniformPointLightCount = glGetUniformLocation(mShaderID, "u_pointLightCount");

	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		char wlocBuff[100] = { '\0' };
		snprintf(wlocBuff, sizeof(wlocBuff), "u_pointLights[%d].base.colour", i);
		uniformPointLight[i].mUniformColor = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].mUniformAmbientIntensity = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].mUniformDiffuseIntensity = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_pointLights[%d].position", i);
		uniformPointLight[i].mUniformPosition = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_pointLights[%d].constant", i);
		uniformPointLight[i].mUniformConstant = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_pointLights[%d].linear", i);
		uniformPointLight[i].mUniformLinear = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_pointLights[%d].exponent", i);
		uniformPointLight[i].mUniformExponent = glGetUniformLocation(mShaderID, wlocBuff);
	}

	mUniformSpotLightCount = glGetUniformLocation(mShaderID, "u_spotLightCount");

	for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
		char wlocBuff[100] = { '\0' };
		snprintf(wlocBuff, sizeof(wlocBuff), "u_spotLights[%d].base.base.colour", i);
		uniformSpotLight[i].mUniformColor = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].mUniformAmbientIntensity = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].mUniformDiffuseIntensity = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_spotLights[%d].base.position", i);
		uniformSpotLight[i].mUniformPosition = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_spotLights[%d].base,constant", i);
		uniformSpotLight[i].mUniformConstant = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_spotLights[%d].base.linear", i);
		uniformSpotLight[i].mUniformLinear = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_spotLights[%d].base.exponent", i);
		uniformSpotLight[i].mUniformExponent = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_spotLights[%d].direction", i);
		uniformSpotLight[i].mUniformDirection = glGetUniformLocation(mShaderID, wlocBuff);

		snprintf(wlocBuff, sizeof(wlocBuff), "u_spotLights[%d].edge", i);
		uniformSpotLight[i].mUniformEdge = glGetUniformLocation(mShaderID, wlocBuff);
	}
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
