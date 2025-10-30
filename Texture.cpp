#include "Texture.h"

/// <summary>
/// Ĭ�Ϲ��캯��
/// </summary>
Texture::Texture()
	:textureID_(0)
	,width_(0)
	,height_(0)
	,bitDepth_(0)
	,fileLocation_("")
{

}

/// <summary>
/// ʹ�� Texture�����ļ�·������ Texture ����
/// </summary>
/// <param name="fileLoc"></param>
Texture::Texture(const char* fileLoc)
	:textureID_(0)
	, width_(0)
	, height_(0)
	, bitDepth_(0)
	, fileLocation_(fileLoc)
{
	
}

/// <summary>
/// ������ͨ�� texture�ļ�(RGB��ʽ), �Զ����� OpenGL Texture����, �󶨲��ϴ�ͼ������.
/// </summary>
/// <returns>���سɹ����� true, ����ʧ�ܷ��� false</returns>
bool Texture::LoadTexture()
{
	unsigned char *texData = stbi_load(fileLocation_, &width_, &height_, &bitDepth_, 0);
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation_);
		return false;
	}

	glGenTextures(1, &textureID_);
	glBindTexture(GL_TEXTURE_2D, textureID_);

	// ������ģʽ - �ظ�����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// �������ģʽ - ���Թ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);

	return true;
}

/// <summary>
/// ������ͨ��(�� Alpha) texture�ļ�(RGBA��ʽ), �Զ����� OpenGL Texture����, �󶨲��ϴ�ͼ������.
/// </summary>
/// <returns>���سɹ����� true, ����ʧ�ܷ��� false</returns>
bool Texture::LoadTextureA()
{
	unsigned char *texData = stbi_load(fileLocation_, &width_, &height_, &bitDepth_, 0);
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation_);
		return false;
	}

	glGenTextures(1, &textureID_);
	glBindTexture(GL_TEXTURE_2D, textureID_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	return true;
}

/// <summary>
/// ʹ����� Texture ����
/// </summary>
void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID_);
}

/// <summary>
/// �����ͷ� Texture ��Դ
/// </summary>
void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID_);
	textureID_ = 0;
	width_ = 0;
	height_ = 0;
	bitDepth_ = 0;
	fileLocation_ = "";
}

/// <summary>
/// ��������
/// </summary>
Texture::~Texture()
{
	ClearTexture();
}
