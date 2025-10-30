#include "Material.h"

/// <summary>
/// Ĭ�Ϲ��캯��, ������ǿ�������ȳ�ʼ��Ϊ 0����ʾ�޸߹ⷴ��. 
/// </summary>
Material::Material()
	:specularIntensity_(0.0f)
	,shininess_(0.0f)
{

}

/// <summary>
/// ���캯��,ʹ�ø����ľ��淴��ǿ�Ⱥ͹���ȳ�ʼ������
/// </summary>
/// <param name="sIntensity">���淴��ǿ��</param>
/// <param name="shine">�����</param>
Material::Material(GLfloat sIntensity, GLfloat shine)
	:specularIntensity_(sIntensity)
	,shininess_(shine)
{

}

/// <summary>
/// �� Shader ���ݲ��ʵľ��淴��ǿ�Ⱥ͹����
/// </summary>
/// <param name="specularIntensityLocation">Shader ���淴��ǿ�� uniform λ��</param>
/// <param name="shininessLocation">Shader ����� uniform λ��</param>
void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity_);
	glUniform1f(shininessLocation, shininess_);
}

/// <summary>
/// ��������
/// </summary>
Material::~Material()
{
}
