#include "Shader.h"

/// <summary>
/// Ĭ�Ϲ��캯������ʼ������ uniform λ�ü� Shader ���� ID Ϊ 0��
/// </summary>
Shader::Shader()
{
}

/// <summary>
/// ʹ�ö�����ɫ����Ƭ����ɫ��Դ���ַ������� Shader ����
/// </summary>
/// <param name="vertexCode">������ɫ�� GLSL Դ��</param>
/// <param name="fragmentCode">Ƭ����ɫ�� GLSL Դ��</param>
void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

/// <summary>
/// ʹ�ö��㡢���κ�Ƭ����ɫ���ļ�·������ Shader ����
/// </summary>
/// <param name="vertexLocation">������ɫ���ļ�·��</param>
/// <param name="geometryLocation">������ɫ���ļ�·��</param>
/// <param name="fragmentLocation">Ƭ����ɫ���ļ�·��</param>
void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

/// <summary>
/// ʹ�ö��㡢���κ�Ƭ����ɫ���ļ�·������ Shader ����
/// </summary>
/// <param name="vertexLocation">������ɫ���ļ�·��</param>
/// <param name="geometryLocation">������ɫ���ļ�·��</param>
/// <param name="fragmentLocation">Ƭ����ɫ���ļ�·��</param>
void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string geometryString = ReadFile(geometryLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, geometryCode, fragmentCode);
}

/// <summary>
/// ��֤ Shader ���������״̬����Ч�ԡ�
/// </summary>
void Shader::Validate()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(shaderID_);
	glGetProgramiv(shaderID_, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID_, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
}

/// <summary>
/// ��ȡ��ɫ���ļ��������������ַ���
/// </summary>
/// <param name="fileLocation">�ļ�·��</param>
/// <returns>�ļ����������ַ���</returns>
std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

#pragma region  uniform getter
GLuint Shader::GetProjectionLocation()
{
	return uniformProjection_;
}
GLuint Shader::GetModelLocation()
{
	return uniformModel_;
}
GLuint Shader::GetViewLocation()
{
	return uniformView_;
}
GLuint Shader::GetAmbientColourLocation()
{
	return uniformDirectionalLight_.uniformColour_;
}
GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionalLight_.uniformAmbientIntensity_;
}
GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDirectionalLight_.uniformDiffuseIntensity_;
}
GLuint Shader::GetDirectionLocation()
{
	return uniformDirectionalLight_.uniformDirection_;
}
GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity_;
}
GLuint Shader::GetShininessLocation()
{
	return uniformShininess_;
}
GLuint Shader::GetEyePositionLocation()
{
	return uniformEyePosition_;
}
GLuint Shader::GetOmniLightPosLocation()
{
	return uniformOmniLightPos_;
}
GLuint Shader::GetFarPlaneLocation()
{
	return uniformFarPlane_;
}
#pragma endregion

/// <summary>
/// �� directionalLight(ƽ�й�)���Դ��� Shader�� uniform 
/// </summary>
/// <param name="dLight">Ҫ����� directionalLight����ָ��</param>
void Shader::SetDirectionalLight(DirectionalLight * dLight)
{
	dLight->UseLight(uniformDirectionalLight_.uniformAmbientIntensity_, uniformDirectionalLight_.uniformColour_,
		uniformDirectionalLight_.uniformDiffuseIntensity_, uniformDirectionalLight_.uniformDirection_);
}
/// <summary>
/// ����� PointLight (����)������Ӱ��ͼ���� Shader �� uniform 
/// </summary>
/// <param name="pLight">ָ�� PointLight �����ָ��</param>
/// <param name="lightCount">���Դ����</param>
/// <param name="textureUnit">��ʼ����Ԫ���������ڰ���Ӱ��ͼ��</param>
/// <param name="offset">��Ӱ��ͼ�� Uniform �����е�ƫ�������������ֲ�ͬ��Դ���ͣ�</param>
void Shader::SetPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount_, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight_[i].uniformAmbientIntensity_, uniformPointLight_[i].uniformColour_,
			uniformPointLight_[i].uniformDiffuseIntensity_, uniformPointLight_[i].uniformPosition_,
			uniformPointLight_[i].uniformConstant_, uniformPointLight_[i].uniformLinear_, uniformPointLight_[i].uniformExponent_);

		pLight[i].getShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap_[i + offset].shadowMap_, textureUnit + i);
		glUniform1f(uniformOmniShadowMap_[i + offset].farPlane_, pLight[i].GetFarPlane());
	}
}

/// <summary>
/// ������۹�ƣ�SpotLight�������ԺͶ�Ӧ��Ӱ��ͼ��Ϣ���� Shader �� uniform
/// </summary>
/// <param name="sLight">ָ�� SpotLight �����ָ��</param>
/// <param name="lightCount">�۹������</param>
/// <param name="textureUnit">��ʼ����Ԫ���������ڰ���Ӱ��ͼ</param>
/// <param name="offset">��Ӱ��ͼ uniform ����ƫ��������ֹ��������Դ���ͳ�ͻ</param>
void Shader::SetSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount_, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(uniformSpotLight_[i].uniformAmbientIntensity_, uniformSpotLight_[i].uniformColour_,
			uniformSpotLight_[i].uniformDiffuseIntensity_, uniformSpotLight_[i].uniformPosition_, uniformSpotLight_[i].uniformDirection_,
			uniformSpotLight_[i].uniformConstant_, uniformSpotLight_[i].uniformLinear_, uniformSpotLight_[i].uniformExponent_,
			uniformSpotLight_[i].uniformEdge_);

		sLight[i].getShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap_[i + offset].shadowMap_, textureUnit + i);
		glUniform1f(uniformOmniShadowMap_[i + offset].farPlane_, sLight[i].GetFarPlane());
	}
}

/// <summary>
/// �󶨲����ó���������ͨ�������������������
/// </summary>
/// <param name="textureUnit">������Ԫ����</param>
void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture_, textureUnit);
}

/// <summary>
/// ����ƽ�й���Ӱ��ͼ������Ԫ������
/// </summary>
/// <param name="textureUnit">��Ӱ��ͼ����Ԫ����</param>
void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap_, textureUnit);
}

/// <summary>
/// ����ƽ�й�Ĺ��ձ任����light transform����������Ӱ��ͼ����ȼ���
/// </summary>
/// <param name="lTransform">ָ����ձ任����ͨ��Ϊ��Դ��ͼͶӰ���󣩵�ָ��</param>
void Shader::SetDirectionalLightTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform_, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

/// <summary>
/// ���õ��Դ��۹�Ƶ�������������Ӱ�任�������� omni-directional shadow mapping��  
/// ÿ�������Ӧ��������ͼ��һ���� (+X, -X, +Y, -Y, +Z, -Z)
/// </summary>
/// <param name="lightMatrices">����������ͼͶӰ����� vector</param>
void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (size_t i = 0; i < 6; i++)
	{
		glUniformMatrix4fv(uniformLightMatrices_[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

/// <summary>
/// ���õ�ǰ��ɫ������
/// </summary>
void Shader::UseShader()
{
	glUseProgram(shaderID_);
}

/// <summary>
/// ������ɫ����Դ
/// </summary>
void Shader::ClearShader()
{
	if (shaderID_ != 0)
	{
		glDeleteProgram(shaderID_);
		shaderID_ = 0;
	}

	uniformModel_ = 0;
	uniformProjection_ = 0;
}

/// <summary>
/// ��������
/// </summary>
Shader::~Shader()
{
	ClearShader();
}

/// <summary>
/// ���붥����ɫ��, Ƭ����ɫ��, ������Ϊ Shader ����
/// </summary>
/// <param name="vertexCode">������ɫ�� GLSL Դ��</param>
/// <param name="fragmentCode">Ƭ����ɫ�� GLSL Դ��</param>
void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID_ = glCreateProgram();

	if (!shaderID_)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID_, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID_, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

/// <summary>
/// ���붥�㡢������Ƭ����ɫ��Դ�룬������Ϊ Shader ����
/// </summary>
/// <param name="vertexCode">������ɫ�� GLSL Դ��</param>
/// <param name="geometryCode">������ɫ�� GLSL Դ��</param>
/// <param name="fragmentCode">Ƭ����ɫ�� GLSL Դ��</param>
void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	shaderID_ = glCreateProgram();

	if (!shaderID_)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID_, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID_, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID_, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

/// <summary>
/// ������ Shader Դ����ӵ���ɫ�������н��б���
/// </summary>
/// <param name="theProgram">Ŀ�� shader����</param>
/// <param name="shaderCode">Ҫ�������ɫ�� GLSL Դ��</param>
/// <param name="shaderType">Ҫ�������ɫ������</param>
void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

/// <summary>
/// �������е��ѱ���� ��ɫ��(Shader) ģ�鵽 ���� Shader ����
/// </summary>
void Shader::CompileProgram() {

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID_);
	glGetProgramiv(shaderID_, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID_, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	uniformProjection_ = glGetUniformLocation(shaderID_, "projection");
	uniformModel_ = glGetUniformLocation(shaderID_, "model");
	uniformView_ = glGetUniformLocation(shaderID_, "view");
	uniformDirectionalLight_.uniformColour_ = glGetUniformLocation(shaderID_, "directionalLight.base.colour");
	uniformDirectionalLight_.uniformAmbientIntensity_ = glGetUniformLocation(shaderID_, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight_.uniformDirection_ = glGetUniformLocation(shaderID_, "directionalLight.direction");
	uniformDirectionalLight_.uniformDiffuseIntensity_ = glGetUniformLocation(shaderID_, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity_ = glGetUniformLocation(shaderID_, "material.specularIntensity");
	uniformShininess_ = glGetUniformLocation(shaderID_, "material.shininess");
	uniformEyePosition_ = glGetUniformLocation(shaderID_, "eyePosition");

	uniformPointLightCount_ = glGetUniformLocation(shaderID_, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
		uniformPointLight_[i].uniformColour_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight_[i].uniformAmbientIntensity_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight_[i].uniformDiffuseIntensity_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight_[i].uniformPosition_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight_[i].uniformConstant_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight_[i].uniformLinear_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight_[i].uniformExponent_ = glGetUniformLocation(shaderID_, locBuff);
	}

	uniformSpotLightCount_ = glGetUniformLocation(shaderID_, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", i);
		uniformSpotLight_[i].uniformColour_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight_[i].uniformAmbientIntensity_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight_[i].uniformDiffuseIntensity_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight_[i].uniformPosition_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight_[i].uniformConstant_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight_[i].uniformLinear_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight_[i].uniformExponent_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight_[i].uniformDirection_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight_[i].uniformEdge_ = glGetUniformLocation(shaderID_, locBuff);
	}

	uniformDirectionalLightTransform_ = glGetUniformLocation(shaderID_, "directionalLightTransform");
	uniformTexture_ = glGetUniformLocation(shaderID_, "theTexture");
	uniformDirectionalShadowMap_ = glGetUniformLocation(shaderID_, "directionalShadowMap");

	uniformOmniLightPos_ = glGetUniformLocation(shaderID_, "lightPos");
	uniformFarPlane_ = glGetUniformLocation(shaderID_, "farPlane");

	for (size_t i = 0; i < 6; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
		uniformLightMatrices_[i] = glGetUniformLocation(shaderID_, locBuff);
	}

	for (size_t i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
		uniformOmniShadowMap_[i].shadowMap_ = glGetUniformLocation(shaderID_, locBuff);

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
		uniformOmniShadowMap_[i].farPlane_ = glGetUniformLocation(shaderID_, locBuff);
	}
}
