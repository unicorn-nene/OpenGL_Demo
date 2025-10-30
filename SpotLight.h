#pragma once
#include "PointLight.h"

/// <summary>
/// SpotLight(�۹��)��, �̳��� PointLight,
/// ����ģ��۹��Ч��: ��һ����λ����ָ�����������, ����ֻ����׶�������ڵ�ƬԪ�Żᱻ����.
/// </summary>
class SpotLight :
	public PointLight
{
public:
	SpotLight();

	SpotLight(GLuint shadowWidth, GLuint shadowHeight,
			  GLfloat near, GLfloat far, 
			  GLfloat red, GLfloat green, GLfloat blue,
			  GLfloat aIntensity, GLfloat dIntensity,
			  GLfloat xPos, GLfloat yPos, GLfloat zPos,
			  GLfloat xDir, GLfloat yDir, GLfloat zDir,
			  GLfloat con, GLfloat lin, GLfloat exp,
			  GLfloat edg);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		          GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		          GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		          GLuint edgeLocation);

	void SetFlash(glm::vec3 pos, glm::vec3 dir);

	void Toggle() { isOn_ = !isOn_; }

	~SpotLight();

private:
	glm::vec3 direction_{};				 ///< �۹�Ʒ�������(������׶��ָ��)
	GLfloat edge_{};                     ///< ����׶��Ե��λ(��λ:��), ��ʾ���յ���Ч����, �����ýǶȵ������Ѹ��˥��.
	GLfloat procEdge_{};				 ///< Ԥ�����Ĺ��ձ�Ե����ֵ(��������ɫ���п��ٱȽϹ��߽Ƕ�)

	bool isOn_{};						 ///< �۹���Ƿ�����true ��ʾ���⣬false ��ʾ�رգ�
};

