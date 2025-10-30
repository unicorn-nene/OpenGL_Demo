#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "ShadowMap.h"

/// <summary>
/// ��Դ����, ������Դ��ɫ,������ǿ��,������ǿ�Ȼ�����Ϣ
/// </summary>
class Light
{
public:
	Light();
	Light(GLuint shadowWidth, GLuint shadowHeight, 
			GLfloat red, GLfloat green, GLfloat blue,
			GLfloat aIntensity, GLfloat dIntensity);

	ShadowMap* getShadowMap() { return shadowMap_; }

	~Light();

protected:
	glm::vec3 colour_{};					 ///< ��Դ��ɫ(RGB)
	GLfloat ambientIntensity_{};			 ///< ������ǿ��
	GLfloat diffuseIntensity_{};			 ///< ������ǿ��

	glm::mat4 lightProj_{};					 ///< ��ԴͶӰ����, ������Ӱ����

	ShadowMap* shadowMap_{};				 ///< ��Ӱ��ͼ����ָ��(�������)
};

