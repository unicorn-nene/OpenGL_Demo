#pragma once
#include "Light.h"

#include <vector>

#include "OmniShadowMap.h"

/// <summary>
/// ���Դ�Ǵ�һ���������з�����ȷ�����ߵĹ�Դ������λ�����Ժ�˥������
/// </summary>
class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(GLuint shadowWidth, GLuint shadowHeight,
			   GLfloat near, GLfloat far,
			   GLfloat red, GLfloat green, GLfloat blue,
			   GLfloat aIntensity, GLfloat dIntensity,
			   GLfloat xPos, GLfloat yPos, GLfloat zPos,
			   GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		          GLuint diffuseIntensityLocation, GLuint positionLocation,
		          GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();
	GLfloat GetFarPlane();
	glm::vec3 GetPosition();

	~PointLight();

protected:
	glm::vec3 position_{};   ///< ���Դλ��, ��������ϵ����
	GLfloat constant_{};     ///< ˥������
	GLfloat linear_{};       ///< ˥��һ����
	GLfloat exponent_{};     ///< ˥��������

	GLfloat farPlane_{};     ///< ��Ӱӳ���Զ�ü�ƽ�����
};

