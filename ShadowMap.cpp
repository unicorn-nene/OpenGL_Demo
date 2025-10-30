#include "ShadowMap.h"

/// <summary>
/// Ĭ�Ϲ��캯��
/// </summary>
ShadowMap::ShadowMap()
	:FBO_(0)
	,shadowMap_(0)
{

}

/// <summary>
/// ��ʼ����Ӱ��ͼ(shadow map) ,֡����(FBO) �� �������
/// </summary>
/// <param name="width">��Ӱ��ͼ�Ŀ�ȡ�</param>
/// <param name="height">��Ӱ��ͼ�ĸ߶ȡ�</param>
/// <returns>��֡���崴���ɹ����� true������ʧ�ܷ��� false</returns>
bool ShadowMap::Init(unsigned int width, unsigned int height)
{
	shadowWidth_ = width;
	shadowHeight_ = height;

	// ����֡�������
	glGenFramebuffers(1, &FBO_);

	// ���������������
	glGenTextures(1, &shadowMap_);
	glBindTexture(GL_TEXTURE_2D, shadowMap_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth_, shadowHeight_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	// ����������� ���Բ�ֵ
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ���������Ʒ�ʽ �߽�ü�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// �趨�߽���ɫΪ��ɫ, ���ڳ�����Χ�Ĳ���
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// �� �����ӵ� FBO����ȸ���(Depth attachment)
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO_);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap_, 0);

	// ��ֹ��ɫ���, ֻ��Ⱦ���
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// ���֡�����Ƿ�����
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %d\n", Status);
		return false;
	}

	return true;
}

/// <summary>
/// �󶨵�ǰ��Ӱ��ͼ��֡�������FBO���Խ��������Ⱦ. ���ú󣬺����Ļ��������д�����Ӱ��ͼ
/// ���� Wirte() -> ��Ⱦ����(��Դ�ӽ�),���������ͼ
/// </summary>
void ShadowMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO_);
}

/// <summary>
/// ����Ⱦ��Ӱʱ������Ӱ��ͼ�󶨵�ָ��������Ԫ��Texture Unit). ��������ƬԪ��ɫ��������
/// ���� Read() -> Shader �������, �Ƚ�ƬԪ������Դ����ж��Ƿ�����Ӱ��
/// </summary>
/// <param name="TextureUnit">Ҫ�󶨵�����Ԫ</param>
void ShadowMap::Read(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMap_);
}

/// <summary>
/// ��������
/// </summary>
ShadowMap::~ShadowMap()
{
	if (FBO_)
	{
		glDeleteFramebuffers(1, &FBO_);
	}

	if (shadowMap_)
	{
		glDeleteTextures(1, &shadowMap_);
	}
}
