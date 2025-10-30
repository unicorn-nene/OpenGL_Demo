#pragma once

#include <GL\glew.h>

#include "CommonValues.h"

/// <summary>
/// ��װ OpenGL Texture(����)����ļ���, ʹ��������.
/// ֧����ͨ Texture �� ͸��ͨ���� Texture ����
/// </summary>
class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
	bool LoadTextureA();

	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID_{};				///< OpenGL ������� ID
	int width_;							///< texture(����) ���ؿ��
	int height_;						///< texture(����) ���ظ߶�
	int bitDepth_;						///< texture(����) �������

	const char* fileLocation_{};		///< texture(����) �ļ�·��
};

