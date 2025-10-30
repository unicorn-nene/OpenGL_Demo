#include "Light.h"

/// <summary>
/// Ĭ�Ϲ��캯������ʼ����ɫ��Դ������Ӱ��ͼ��
/// </summary>
Light::Light()
	:colour_(glm::vec3(1.0f, 1.0f, 1.0f))
	,ambientIntensity_(1.0f)
	,diffuseIntensity_(0.0f)
{

}

/// <summary>
/// ������������Ӱ��Ⱦ�Ĺ�Դ
/// </summary>
/// <param name="shadowWidth">��Ӱ��ͼ���</param>
/// <param name="shadowHeight">��Ӱ��ͼ�߶�</param>
/// <param name="red">��Դ��ɫ R ����</param>
/// <param name="green">��Դ��ɫ G ����</param>
/// <param name="blue">��Դ��ɫ B ����</param>
/// <param name="aIntensity">���������ǿ��</param>
/// <param name="dIntensity">����������ǿ��</param>
Light::Light(GLuint shadowWidth, GLuint shadowHeight,
			 GLfloat red, GLfloat green, GLfloat blue,
			 GLfloat aIntensity, GLfloat dIntensity)
	:colour_(glm::vec3(red, green, blue))
	,ambientIntensity_(aIntensity)
	,diffuseIntensity_(dIntensity)
{
	shadowMap_ = new ShadowMap();
	shadowMap_->Init(shadowWidth, shadowHeight);
}

/// <summary>
/// ��������
/// </summary>
Light::~Light()
{
}
