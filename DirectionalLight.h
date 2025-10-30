#pragma once
#include "Light.h"

/// <summary>
/// �����/�����/ƽ�й� ��Դ�ࣺ����ģ��̫��������Զ��Դ��
/// ���߷���һ�£��������λ�øı䡣
/// </summary>
class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, 
					GLfloat red, GLfloat green, GLfloat blue,
					GLfloat aIntensity, GLfloat dIntensity,
					GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation, GLint diffuseIntensityLocation, GLint directionLocation);

	glm::mat4 CalculateLightTransform();

	~DirectionalLight();

private:
	glm::vec3 direction_{};				///< ��Դ���� (������ָ���Դ�ķ���ȡ��)
};

