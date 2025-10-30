#include "DirectionalLight.h"

/// <summary>
/// Ĭ�Ϲ����ɫ����⣬����Ϊ (0, -1, 0), ����������
/// </summary>
DirectionalLight::DirectionalLight()
	:Light()
	,direction_(glm::vec3(0.0f, -1.0f, 0.0f))
{
	
}

/// <summary>
/// ���������Դ����ʼ����Ӱ��ͼ
/// </summary>
/// <param name="shadowWidth">��Ӱ��ͼ���</param>
/// <param name="shadowHeight">��Ӱ��ͼ�߶�</param>
/// <param name="red">��Դ��ɫ R ����</param>
/// <param name="green">��Դ��ɫ G ����</param>
/// <param name="blue">��Դ��ɫ B ����</param>
/// <param name="aIntensity">������ǿ��</param>
/// <param name="dIntensity">�������ǿ��</param>
/// <param name="xDir">�ⷽ�� X ����</param>
/// <param name="yDir">�ⷽ�� Y ����</param>
/// <param name="zDir">�ⷽ�� Z ����</param>
DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, 
									GLfloat red, GLfloat green, GLfloat blue,
									GLfloat aIntensity, GLfloat dIntensity,
									GLfloat xDir, GLfloat yDir, GLfloat zDir) 
	: Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
	, direction_(glm::vec3(xDir, yDir, zDir))
{
	// �����ʹ������ͶӰ
	lightProj_ = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

/// <summary>
/// �� Shader �ϴ���Դ���ݵ� uniform ����
/// </summary>
/// <param name="ambientIntensityLocation">������ǿ�� uniform</param>
/// <param name="ambientColourLocation">����ɫ uniform</param>
/// <param name="diffuseIntensityLocation">�������ǿ�� uniform</param>
/// <param name="directionLocation">�ⷽ�� uniform</param>
void DirectionalLight::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation, GLint diffuseIntensityLocation, GLint directionLocation)
{
	glUniform3f(ambientColourLocation, colour_.x, colour_.y, colour_.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity_);

	glUniform3f(directionLocation, direction_.x, direction_.y, direction_.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity_);
}

/// <summary>
/// ��ȡ�����������Ӱ��Ⱦ�Ĺ�Դ�任����, �þ�����������ת������Դ�ӽǣ�Light Space��.
/// ��Դ�任���� = ��ԴͶӰ���� * ��Դ��ͼ����
/// </summary>
/// <returns>Light Space Matrix����Դ�ռ�任����</returns>
glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj_ * glm::lookAt(-direction_, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

/// <summary>
/// ���������������ͷ� shadowMap_��
/// </summary>
DirectionalLight::~DirectionalLight()
{
}
