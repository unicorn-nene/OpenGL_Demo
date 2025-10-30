#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

/// <summary>
/// Shader �ฺ�����, ����, ���� OpenGL��ɫ������, ����ȡ/������ɫ����ʹ�õ� uniform ����λ��.
/// </summary>
class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);

	void Validate();

	std::string ReadFile(const char* fileLocation);

	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight* sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

	void UseShader();
	void ClearShader();

	~Shader();

#pragma region uniform_getter
	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyePositionLocation();
	GLuint GetOmniLightPosLocation();
	GLuint GetFarPlaneLocation();
#pragma endregion

private:
#pragma region private function
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	void CompileProgram();
#pragma endregion

private:
#pragma region light_count
	int pointLightCount_{};								///< ��ǰ���Դ����
	int spotLightCount_{};								///< ��ǰ�۹������
#pragma endregion

	GLuint shaderID_{};									///< ��ǰ��ɫ���������� OpenGL ID

#pragma region uniform
	GLuint uniformProjection_{};						///< ͶӰ���� uniform λ��
	GLuint uniformModel_{};								///< ģ�;��� uniform λ��
	GLuint uniformView_{};								///< ��ͼ���� uniform λ��
	GLuint uniformEyePosition_{};						///< �����λ�� uniform λ��
	GLuint uniformSpecularIntensity_{};					///< ���淴��ǿ�� uniform 
	GLuint uniformShininess_{};							///< ����߹�ϵ�� uniform 
	GLuint uniformTexture_{};							///< ���� uniform 
	GLuint uniformDirectionalShadowMap_{};				///< ƽ�й���Ӱ��ͼ sampler uniform 
	GLuint uniformDirectionalLightTransform_{};			///< ƽ�й���Ӱ�任���� uniform
	GLuint uniformOmniLightPos_{};						///< ���Դ��Ӱ��ͼ��Դλ�� uniform
	GLuint uniformFarPlane_{};							///< ���Դ��Ӱ��ͼԶ���� uniform

	GLuint uniformLightMatrices_[6]{};					///< ��������Ӱ��ͼ��Ӧ 6 ���ӽǵĹ�Դ�任���� uniform�����Դ��Ӱ��
#pragma endregion

#pragma region uniformDirectionalLight_struct
	///< ƽ�й�ṹ��
	struct
	{
		GLuint uniformColour_{};						///< ����ɫ
		GLuint uniformAmbientIntensity_{};				///< ������ǿ��
		GLuint uniformDiffuseIntensity_{};				///< ������ǿ��

		GLuint uniformDirection_{};						///< ���շ���
	}uniformDirectionalLight_;
#pragma endregion

#pragma region uniformPointLight_struct
	GLuint uniformPointLightCount_{};					///< ���Դ���� uniform
	///< ���Դ(�ṹ��)���飨֧�ֶ��Դ�� uniform ��������
	struct
	{
		GLuint uniformColour_{};						///< ����ɫ
		GLuint uniformAmbientIntensity_{};				///< ������ǿ��
		GLuint uniformDiffuseIntensity_{};				///< ������ǿ��

		GLuint uniformPosition_{};						///< ��Դλ��
		GLuint uniformConstant_{};						///< ˥��������
		GLuint uniformLinear_{};						///< ˥��һ����
		GLuint uniformExponent_{};						///< ˥��������
	} uniformPointLight_[MAX_POINT_LIGHTS];
#pragma endregion

#pragma region uniformSpotLight_struct
	GLuint uniformSpotLightCount_{};					///< �۹������ uniform
	///< �۹��(�ṹ��)���飨֧�ֶ��Դ�� uniform ��������
	struct
	{
		GLuint uniformColour_{};						///< ����ɫ
		GLuint uniformAmbientIntensity_{};				///< ������ǿ��
		GLuint uniformDiffuseIntensity_{};				///< ������ǿ��

		GLuint uniformPosition_{};						///< ��Դλ��
		GLuint uniformConstant_{};						///< ˥��������
		GLuint uniformLinear_{};						///< ˥��һ����
		GLuint uniformExponent_{};						///< ˥��������

		GLuint uniformDirection_{};						///< �۹ⷽ��
		GLuint uniformEdge_{};							///< �۹��Ե˥���ǣ�cos(��)��
	}uniformSpotLight_[MAX_SPOT_LIGHTS];
#pragma endregion

#pragma region uniformOmniShadowMap_struct
	///< ������Ӱ��ͼ(�ṹ��) uniform ����
	struct
	{
		GLuint shadowMap_{};							///< ������Ӱ��ͼ���� uniform
		GLuint farPlane_{};								///< ��Ӱ�����е������ȷ�Χ
	}uniformOmniShadowMap_[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];
#pragma endregion
};

