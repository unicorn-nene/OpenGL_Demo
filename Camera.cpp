#include "Camera.h"

/// <summary>
/// Ĭ�Ϲ��캯��, ʹ��ԭ��λ��,Ĭ�Ϸ����ʼ��
/// </summary>
Camera::Camera()
{}

/// <summary>
/// ָ����ʼλ�á����� Up��Yaw��Pitch���ƶ��ٶ�����ת�ٶȵĹ��캯����
/// </summary>
/// <param name="startPosition">�������ʼλ��</param>
/// <param name="startUp">��������ϵ�� Up ����</param>
/// <param name="startYaw">��ʼƫ���ǣ��� Y �ᣩ</param>
/// <param name="startPitch">��ʼ�����ǣ��� X �ᣩ</param>
/// <param name="startMoveSpeed">�����ƶ��ٶ�</param>
/// <param name="startTurnSpeed">�����ת������</param>
Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
	:position_(startPosition)
	, front_(glm::vec3(0.0f, 0.0f, -1.0f))
	,worldUp_(startUp)
	,yaw_(startYaw)
	,pitch_(startPitch)
	,moveSpeed_(startMoveSpeed)
	,turnSpeed_(startTurnSpeed)
{
	update();
}

/// <summary>
/// ���ݼ�������(ǰ������)���������λ��
/// </summary>
/// <param name="keys">����״̬����</param>
/// <param name="deltaTime">ÿ֮֡��ļ��ʱ��</param>
void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed_ * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position_ += front_ * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position_ -= front_ * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position_ -= right_ * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position_ += right_ * velocity;
	}
}

/// <summary>
/// ��������ƶ������� Camera �� yaw �� pitch, �Ӷ��ı� Camera ����
/// </summary>
/// <param name="xChange">��� X ����仯��</param>
/// <param name="yChange">��� Y ����仯��</param>
void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed_;
	yChange *= turnSpeed_;

	yaw_ += xChange;
	pitch_ += yChange;

	if (pitch_ > 89.0f)
	{
		pitch_ = 89.0f;
	}

	if (pitch_ < -89.0f)
	{
		pitch_ = -89.0f;
	}

	update();
}

/// <summary>
/// ��ȡ������Ⱦ�� view (��ͼ)����
/// </summary>
/// <returns>view (��ͼ)����</returns>
glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position_, position_ + front_, up_);
}

/// <summary>
/// ��ȡ Camera ��ǰλ��
/// </summary>
/// <returns>Camera������ռ��е�λ������</returns>
glm::vec3 Camera::getCameraPosition()
{
	return position_;
}

/// <summary>
/// ��ȡ Camera �Ĺ۲췽��
/// </summary>
/// <returns>Camera����ĵ�λ��������</returns>
glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front_);
}

/// <summary>
/// ���� yaw �� pitch ���� front/right/up ����
/// </summary>
void Camera::update()
{
	// ʹ��ŷ���Ǽ��� Camera ��ǰ����
	front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_.y = sin(glm::radians(pitch_));
	front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(front_);

	// �ҷ��� = ǰ���� �� �����Ϸ���
	right_ = glm::normalize(glm::cross(front_, worldUp_));
	// �Ϸ��� = �ҷ��� �� ǰ����
	up_ = glm::normalize(glm::cross(right_, front_));
}

/// <summary>
/// ��������
/// </summary
Camera::~Camera()
{
}
