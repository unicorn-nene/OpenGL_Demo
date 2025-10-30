#pragma once

#include <GL\glew.h>

/// <summary>
/// ��װһ�� Mesh(����) ����Ĵ���,��Ⱦ���������
/// ʹ�� VAO, VBO, IBO ��ʽ�� GPU �ϴ� vertex(����)�� index(����)����.
/// </summary>
class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO_{};			///< �����������Vertex Array Object��
	GLuint VBO_{};			///< ���㻺�����Vertex Buffer Object��
	GLuint IBO_{};			///< �����������Index Buffer Object��

	GLsizei indexCount_{};	///< ����������������Ⱦ����
};

