#pragma once

#include "stdio.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

/// <summary>
/// Window �� ��װ GLFW �����������¼�
/// �� Window �ฺ��:
/// - ��������ʼ�� OpenGL ����
/// - ����֡��������С
/// - ��¼��������״̬
/// - ��¼����ƶ�����
/// - �ṩ���ڹر��ж��뻺������������
/// </summary>
class Window
{
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth_; }
	GLint getBufferHeight() { return bufferHeight_; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow_); }

	bool* getsKeys() { return keys_; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow_); }

	~Window();

private:
	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

private:
	GLFWwindow* mainWindow_;			///< ������ָ��

	GLint width_{};						///< ���ڿ��
	GLuint height_{};					///< ���ڸ߶�
	GLint bufferWidth_{};				///< ʵ��֡���������
	GLint bufferHeight_{};				///< ʵ��֡�������߶�

	bool keys_[1024]{ false };			///< ���̰���״̬��

	GLfloat lastX_{};					///< ��һ����� X ����
	GLfloat lastY_{};					///< ��һ����� Y ����
	GLfloat xChange_{};					///< ��� X ƫ����
	GLfloat yChange_{};					///< ��� Y ƫ����
	bool mouseFirstMoved_{};			///< �ж��Ƿ��һ���ɼ��������
};