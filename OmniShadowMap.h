#pragma once
#include "ShadowMap.h"

/// <summary>
/// OmniShadowMap �̳� ShadowMap ��,���ڼ�¼���Դ����Ӱ��ͼ
/// OmniShadowMap �� ʹ����������ͼ(cube Map),�ܼ�¼�ӹ�Դ������ 6������������Ϣ
/// </summary>
class OmniShadowMap :
	public ShadowMap
{
public:
	OmniShadowMap();

	bool Init(unsigned int width, unsigned int height);

	void Write();

	void Read(GLenum TextureUnit);

	~OmniShadowMap();
};

