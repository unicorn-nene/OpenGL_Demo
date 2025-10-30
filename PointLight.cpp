#include "PointLight.h"

/// <summary>
/// Ĭ�Ϲ��캯��, ��ʼ��һ��λ��ԭ��, ��˥���İ�ɫ���Դ
/// </summary>
PointLight::PointLight() 
	: Light()
	, position_(glm::vec3(0.0f, 0.0f, 0.0f))
	, constant_(1.0f)	
	, linear_(0.0f)
	, exponent_(0.0f)
{
}

/// <summary>
/// ����һ������Ӱ�ĵ��Դ��
/// ���Դʹ����������ͼ��Cubemap����Ⱦ��Ӱ����Ҫ 6 ���ӽǷ���
/// </summary>
/// <param name="shadowWidth">��Ӱ��ͼ���</param>
/// <param name="shadowHeight">��Ӱ��ͼ�߶�</param>
/// <param name="near">��Դ������</param>
/// <param name="far">��ԴԶ���棨������Ӱ������ȷ�Χ��</param>
/// <param name="red">����ɫ R ����</param>
/// <param name="green">����ɫ G ����</param>
/// <param name="blue">����ɫ B ����</param>
/// <param name="aIntensity">������ǿ��</param>
/// <param name="dIntensity">�������ǿ��</param>
/// <param name="xPos">��Դλ�� X ����</param>
/// <param name="yPos">��Դλ�� Y ����</param>
/// <param name="zPos">��Դλ�� Z ����</param>
/// <param name="con">˥��������</param>
/// <param name="lin">˥��������</param>
/// <param name="exp">˥��ָ��������</param>
PointLight::PointLight(GLuint shadowWidth, GLuint shadowHeight,
					   GLfloat near, GLfloat far, 
					   GLfloat red, GLfloat green, GLfloat blue,
					   GLfloat aIntensity, GLfloat dIntensity, 
					   GLfloat xPos, GLfloat yPos, GLfloat zPos, 
					   GLfloat con, GLfloat lin, GLfloat exp) 
	: Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
	, position_(glm::vec3(xPos, yPos, zPos))
	, constant_(con)
	, linear_(lin)
	, exponent_(exp)
	, farPlane_(far)
{
	// ����ͶӰ����: ���Դ��Ҫ͸��ͶӰ(90���ӽ��γ�������� 6������)
	float aspect = static_cast<float>(shadowWidth) / static_cast<float>(shadowHeight);		 // ������ͼ�Ŀ�߱� 
	lightProj_ = glm::perspective(glm::radians(90.0f), aspect, near, far);	     // ����ͶӰ����

	// ��������ȫ����Ӱ��ͼ�� ShadowMap
	shadowMap_ = new OmniShadowMap();
	shadowMap_->Init(shadowWidth, shadowHeight);
}

/// <summary>
/// �����Դ�����Դ��� Shader��ͨ�� uniform ������
/// </summary>
/// <param name="ambientIntensityLocation">������ǿ�� uniform λ��</param>
/// <param name="ambientColourLocation">����ɫ uniform λ��</param>
/// <param name="diffuseIntensityLocation">������ǿ�� uniform λ��</param>
/// <param name="positionLocation">��Դλ�� uniform λ��</param>
/// <param name="constantLocation">˥�������� uniform λ��</param>
/// <param name="linearLocation">˥�������� uniform λ��</param>
/// <param name="exponentLocation">˥�������� uniform λ��</param>
void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation,
						  GLuint positionLocation,GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	glUniform3f(ambientColourLocation, colour_.x, colour_.y, colour_.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity_);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity_);

	glUniform3f(positionLocation, position_.x, position_.y, position_.z);
	glUniform1f(constantLocation, constant_);
	glUniform1f(linearLocation, linear_);
	glUniform1f(exponentLocation, exponent_);
}

/// <summary>
/// ������������Ӱӳ������� 6����Դ��ͼ�任����
/// ÿ�������Ӧһ����
/// </summary>
/// <returns>6����Դ��ͼ������ɵ�����</returns>
std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;
	//+x, -x
	lightMatrices.push_back(lightProj_ * glm::lookAt(position_, position_ + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj_ * glm::lookAt(position_, position_ + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

	//+y, -y
	lightMatrices.push_back(lightProj_ * glm::lookAt(position_, position_ + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightMatrices.push_back(lightProj_ * glm::lookAt(position_, position_ + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));

	//+z, -z
	lightMatrices.push_back(lightProj_ * glm::lookAt(position_, position_ + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj_ * glm::lookAt(position_, position_ + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return lightMatrices;
}

GLfloat PointLight::GetFarPlane()
{
	return farPlane_;
}
glm::vec3 PointLight::GetPosition()
{
	return position_;
}

/// <summary>
///��������
/// </summary>
PointLight::~PointLight()
{
}
