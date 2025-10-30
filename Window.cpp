#include "Window.h"


/// <summary>
/// Ĭ�Ϲ��캯��, ���ڴ�С����Ϊ 800 * 600
/// </summary>
Window::Window()
	:width_(800)
	, height_(600)
	, lastX_(0.0f)
	, lastY_(0.0f)
	, xChange_(0.0f)
	, yChange_(0.0f)
	, mouseFirstMoved_(true)
{
	
}

/// <summary>
/// ���캯��,�Զ��崰�ڿ����߶�
/// </summary>
/// <param name="windowWidth">���ڿ��</param>
/// <param name="windowHeight">���ڸ߶�</param>
Window::Window(GLint windowWidth, GLint windowHeight)
	:width_(windowWidth)
	, height_(windowHeight)
	, lastX_(0.0f)
	, lastY_(0.0f)
	, xChange_(0.0f)
	, yChange_(0.0f)
	, mouseFirstMoved_(true)
{
	
}

/// <summary>
///  ��ʼ�� GLFW, GLEW, �������ڲ�ע��ص�����
/// </summary>
/// <returns>���� 0 ��ʾ�ɹ�, ���� 1 ��ʾʧ��</returns>
int Window::Initialise()
{
	if (!glfwInit())
	{
		printf("Error Initialising GLFW");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW Windows Properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatiblity
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow_ = glfwCreateWindow(width_, height_, "Test Window", NULL, NULL);
	if (!mainWindow_)
	{
		printf("Error creating GLFW window!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mainWindow_, &bufferWidth_, &bufferHeight_);

	// Set the current context
	glfwMakeContextCurrent(mainWindow_);

	// Handle Key + Mouse Input
	createCallbacks();
	glfwSetInputMode(mainWindow_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension access
	glewExperimental = GL_TRUE;

	// init GLEW
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow_);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Create Viewport
	glViewport(0, 0, bufferWidth_, bufferHeight_);

	glfwSetWindowUserPointer(mainWindow_, this);

	return 0;
}

/// <summary>
/// ���(ÿ֡)�ƶ���ˮƽ����
/// </summary>
/// <returns></returns>
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange_;
	xChange_ = 0.0f;
	return theChange;
}

/// <summary>
/// ���(ÿ֡)�ƶ��Ĵ�ֱ����
/// </summary>
/// <returns></returns>
GLfloat Window::getYChange()
{
	GLfloat theChange = yChange_;
	yChange_ = 0.0f;
	return theChange;
}

/// <summary>
/// ��������
/// </summary>
Window::~Window()
{
	glfwDestroyWindow(mainWindow_);
	glfwTerminate();
}


/// =================== private function ===========================

/// <summary>
/// ע�������¼��ص�����, �������̺�����ƶ��¼�.
/// �ú�������� GLEW �Ļص�����, ������������¼�����������ľ�̬������Ա
/// </summary>
void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow_, handleKeys);
	glfwSetCursorPosCallback(mainWindow_, handleMouse);
}

/// <summary>
/// �Զ��� GLFW �����¼��ص�����
/// ������̰����������ͷ��¼�,���� Window���еİ���״̬����
/// </summary>
/// <param name="window">�����¼��� GLFW ����ָ��</param>
/// <param name="key">�����¼��İ���ֵ, ��Ӧ GLFW_KEY_*</param>
/// <param name="code">ɨ����(һ�㲻ʹ��)</param>
/// <param name="action">��������: GLFW_PRESS���� GLFW_RELEASE</param>
/// <param name="mode">���ΰ���״̬,��: shift�� control</param>
void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys_[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys_[key] = false;
		}
	}
}

/// <summary>
/// GLFW ����ƶ��¼��ص�����.
/// ���㵱ǰ���λ�����ϴ�λ�ò�ֵ, ��Ҫ��������ӽ���ת����
/// </summary>
/// <param name="window">�����¼��� GLFW����ָ��</param>
/// <param name="xPos">��굱ǰ�� X ����(��� OpenGL����)</param>
/// <param name="yPos">��굱ǰ�� Y ����(��� OpenGL����)</param>
void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved_)
	{
		theWindow->lastX_ = static_cast<GLfloat>(xPos);
		theWindow->lastY_ = static_cast<GLfloat>(yPos);
		theWindow->mouseFirstMoved_ = false;
		return; // ��һ��ֻ��¼���������ֵ
	}

	// �ȼ����ֵ
	theWindow->xChange_ = static_cast<GLfloat>(xPos - theWindow->lastX_);
	theWindow->yChange_ = static_cast<GLfloat>(theWindow->lastY_ - yPos); // ��ת Y �ӽǿ���

	// �ٸ�����һ������
	theWindow->lastX_ = static_cast<GLfloat>(xPos);
	theWindow->lastY_ = static_cast<GLfloat>(yPos);

	//if (theWindow->mouseFirstMoved_)
	//{
	//	theWindow->lastX_ = xPos;
	//	theWindow->lastY_ = yPos;
	//	theWindow->mouseFirstMoved_ = false;
	//	return; //��һ��ֻ��ʼ�����������ֵ
	//}

	//theWindow->xChange_ = xPos - theWindow->lastX_;
	//theWindow->yChange_ = theWindow->lastY_ - yPos; // ��ת Y �ӽǿ���

	//theWindow->lastX_ = xPos;
	//theWindow->lastY_ = yPos;
}


