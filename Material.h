#pragma once

#include <GL\glew.h>

/// <summary>
/// ��ʾһ�����ʣ�Material�����󣬶�������������ڸ߹⣨specular���׶εķ������ԡ�
/// ������Ҫ������ Phong ����ģ������������ġ����淴��ǿ�ȡ��͡�����ȡ�
/// </summary>
class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();

private: 
	GLfloat specularIntensity_{};		///< ���ʾ��淴��ǿ��, ���Ʒ�����ǿ��,ֵԽ��Խ��
	GLfloat shininess_{};				///< ���ʹ����, ���Ƹ߹�ļ��г̶�,ֵԽ��߹�����ԽС��Խ����.
};

