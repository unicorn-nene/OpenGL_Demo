#include "SpotLight.h"

/// <summary>
/// Ĭ�Ϲ��캯��
/// ��ʼ��Ϊ �ر�״̬, ��ʼ���߷������·�, ��˥��, ��ɫ��.
/// </summary>
SpotLight::SpotLight() 
	: PointLight()
	, direction_(glm::vec3(0.0f, -1.0f, 0.0f))
	, edge_(0.0f)
	, procEdge_(cosf(glm::radians(edge_)))
	, isOn_(true)
{
}

/// <summary>
/// ���캯��������������Ӱ��ͼ֧�ֵľ۹�ơ�
/// </summary>
/// <param name="shadowWidth">��Ӱ��ͼ�Ŀ�ȣ����أ�</param>
/// <param name="shadowHeight">��Ӱ��ͼ�ĸ߶ȣ����أ�</param>
/// <param name="near">͸��ͶӰ�Ľ�ƽ��</param>
/// <param name="far">͸��ͶӰ��Զƽ��</param>
/// <param name="red">��ĺ�ɫ���� R��0~1��</param>
/// <param name="green">�����ɫ���� G��0~1��</param>
/// <param name="blue">�����ɫ���� B��0~1��</param>
/// <param name="aIntensity">������ǿ��</param>
/// <param name="dIntensity">�������ǿ��</param>
/// <param name="xPos">��Դλ�õ� X ����/param>
/// <param name="yPos">��Դλ�õ� Y ����</param>
/// <param name="zPos">��Դλ�õ� Z ����</param>
/// <param name="xDir">���߷��������� X ����</param>
/// <param name="yDir">���߷��������� Y ����</param>
/// <param name="zDir">���߷��������� Z ����</param>
/// <param name="con">����˥��ϵ����constant��</param>
/// <param name="lin">����˥��ϵ����linear��</param>
/// <param name="exp">ָ��˥��ϵ����exponent��</param>
/// <param name="edg">����׶��Ե�ǣ���λ���ȣ�, �Ƕ�ԽС������Խ����</param>
SpotLight::SpotLight(GLuint shadowWidth, GLuint shadowHeight,
					 GLfloat near, GLfloat far, 
					 GLfloat red, GLfloat green, GLfloat blue,
					 GLfloat aIntensity, GLfloat dIntensity, 
					 GLfloat xPos, GLfloat yPos, GLfloat zPos, 
					 GLfloat xDir, GLfloat yDir, GLfloat zDir, 
					 GLfloat con, GLfloat lin, GLfloat exp, 
					 GLfloat edg)
	: PointLight(shadowWidth, shadowHeight, near, far, red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp)
	, direction_(glm::normalize(glm::vec3(xDir, yDir, zDir)))
	, edge_(edg)
	, procEdge_(cosf(glm::radians(edge_)))
	, isOn_(true)
{

}

/// <summary>
/// ���۹�ƹ��ղ������ݸ���ɫ������, ��������Ⱦ�׶ΰ󶨾۹�Ƶ����� Uniform��
/// ���ݵƹ��Ƿ�����isOn_������̬���û��������������ǿ��
/// </summary>
/// <param name="ambientIntensityLocation">Uniform - ������ǿ��λ��</param>
/// <param name="ambientColourLocation">Uniform - ��������ɫλ��</param>
/// <param name="diffuseIntensityLocation">Uniform - ������ǿ��λ��</param>
/// <param name="positionLocation">Uniform - ��Դλ��λ��</param>
/// <param name="directionLocation">Uniform - ���߷���λ��</param>
/// <param name="constantLocation">Uniform - ����˥������λ��</param>
/// <param name="linearLocation">Uniform - ����˥������λ��</param>
/// <param name="exponentLocation">Uniform - ָ��˥������λ��</param>
/// <param name="edgeLocation">Uniform - �۹�׶��Ե�ǣ�������ʽ��λ��</param>
void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, 
						 GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, 
						 GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, 
						 GLuint edgeLocation)
{
	glUniform3f(ambientColourLocation, colour_.x, colour_.y, colour_.z);

	// ����ƹ⿪��, ʹ����������ǿ��
	if (isOn_)
	{
		glUniform1f(ambientIntensityLocation, ambientIntensity_);
		glUniform1f(diffuseIntensityLocation, diffuseIntensity_);
	}
	else // �ƹ�ر�, �ر�(�۹��)�ƹ�Ч��
	{
		glUniform1f(ambientIntensityLocation, 0.0f);
		glUniform1f(diffuseIntensityLocation, 0.0f);
	}

	// ���ݹ�Դ�Ŀռ�λ����˥�����������ƹ�ǿ�����˥����
	glUniform3f(positionLocation, position_.x, position_.y, position_.z);
	glUniform1f(constantLocation, constant_);
	glUniform1f(linearLocation, linear_);
	glUniform1f(exponentLocation, exponent_);

	// ���ݹ��շ������Ե˥����ͨ������Ԥ���㣩
	glUniform3f(directionLocation, direction_.x, direction_.y, direction_.z);
	glUniform1f(edgeLocation, procEdge_);
}

/// <summary>
/// ���¾۹�Ƶ�λ���뷽��, ������Ϊ���� ������ƶ�
/// </summary>
/// <param name="pos"></param>
/// <param name="dir"></param>
void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position_ = pos;
	direction_ = dir;
}

/// <summary>
/// ��������
/// </summary>
SpotLight::~SpotLight()
{
}
