#pragma once

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GLFW\glfw3.h>

/// <summary>
/// ��һ�˳� Camera(�����)��, ʵ�ֻ��ڼ��������������ӽ��ƶ�
/// </summary>
class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();

	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	void update();

private:
	glm::vec3 position_{};				///< Camera��ǰλ��
										
	glm::vec3 front_{};					///< Camera��ǰ��������(�� yaw + pitch)����
	glm::vec3 up_{};					///< Camera��ǰ �Ϸ�(Up)����
	glm::vec3 right_{};					///< Camera��ǰ������, right = normalize(cross(front, worldUp))
	glm::vec3 worldUp_{};				///< ��������ϵ �Ϸ�����
										
	GLfloat yaw_{};						///< ƫ���� (ˮƽ��ת)
	GLfloat pitch_{};					///< ������ (��ֱ��ת)
										
	GLfloat moveSpeed_{};				///< �����ƶ��ٶ�
	GLfloat turnSpeed_{};				///< ���������
};

