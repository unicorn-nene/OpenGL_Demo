#pragma once

#include <stdio.h>

#include <GL\glew.h>

/// <summary>
/// ShaderMap(��Ӱ��ͼ)��: ����һ����Ӱ��ͼ(ShadowMap), ʵ����Ӱӳ��(Shadow Mapping)
/// </summary>
class ShadowMap
{
public:
	ShadowMap();

	virtual bool Init(unsigned int width, unsigned int height);

	virtual void Write();
	virtual void Read(GLenum TextureUnit);

	GLuint GetShadowWidth() { return shadowWidth_; }
	GLuint GetShadowHeight() { return shadowHeight_; }

	~ShadowMap();

protected:
	GLuint FBO_{};					 ///< ֡�������(FrameBuffer object)ID
	GLuint shadowMap_{};			 ///< ��Ӱ��ͼ����(Depth Texture)ID

	GLuint shadowWidth_{};			 ///< ��Ӱ��ͼ���
	GLuint shadowHeight_{};			 ///< ��Ӱ��ͼ�߶�
};