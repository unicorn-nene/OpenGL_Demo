#include "OmniShadowMap.h"

/// <summary>
/// Ĭ�Ϲ��캯��
/// </summary>
OmniShadowMap::OmniShadowMap() 
	: ShadowMap()
{
}

/// <summary>
/// ��ʼ�� ��������ͼ
/// </summary>
/// <param name="width">��Ӱ��ͼ���</param>
/// <param name="height">��Ӱ��ͼ�߶�</param>
/// <returns>��ʼ���ɹ����� true, ��ʼ��ʧ�ܷ��� false</returns>
bool OmniShadowMap::Init(unsigned int width, unsigned int height)
{
	shadowWidth_ = width; shadowHeight_ = height;

	glGenFramebuffers(1, &FBO_);

	glGenTextures(1, &shadowMap_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap_);

	// Ϊ������� 6���������������
	for (size_t i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidth_, shadowHeight_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	// ������������ͼ,���Թ��� + ��Ե�ü�
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	// ����������ͼ���ӵ� ֡����� ��ȸ���(depth attachment)
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap_, 0);
	
	// ��ֹ��ɫд��, ֻ��Ⱦ���
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
void OmniShadowMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO_);
}

/// <summary>
/// ����Ⱦ��Ӱʱ������Ӱ��ͼ�󶨵�ָ��������Ԫ��Texture Unit). ��������ƬԪ��ɫ��������
/// ���� Read() -> Shader �������, �Ƚ�ƬԪ������Դ����ж��Ƿ�����Ӱ��
/// </summary>
/// <param name="TextureUnit">Ҫ�󶨵�����Ԫ</param>
void OmniShadowMap::Read(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap_);
}

/// <summary>
/// ��������, ����ʵ�������Դ�ͷ�
/// </summary>
OmniShadowMap::~OmniShadowMap()
{
}
