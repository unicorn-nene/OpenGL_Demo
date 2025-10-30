#include "Mesh.h"

/// <summary>
/// Ĭ�Ϲ��캯������ʼ�� VAO/VBO/IBO ��Դ���Ϊ 0
/// </summary>
Mesh::Mesh()
	:VAO_(0)
	,VBO_(0)
	,IBO_(0)
	,indexCount_(0)
{

}

/// <summary>
///	�����������ݲ��ϴ��� GPU
/// </summary>
/// <param name="vertices">��������</param>
/// <param name="indices">��������</param>
/// <param name="numOfVertices">����������Ԫ������</param>
/// <param name="numofIndices">��������</param>
void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount_ = numOfIndices;

	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);

	glGenBuffers(1, &IBO_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	//  x, y, z,      u, v,	    nx, ny, nz 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);								// (x, y, z)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));	// (u, v)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));	// (nx, ny, nz)
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

/// <summary>
/// ��Ⱦ Mesh ����
/// </summary>
void Mesh::RenderMesh()
{
	glBindVertexArray(VAO_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
	glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/// <summary>
/// ���� VAO/VBO/IBO ��Դ, ʹ�ö���ص���ʼλ��
/// </summary>
void Mesh::ClearMesh()
{
	if (IBO_ != 0)
	{
		glDeleteBuffers(1, &IBO_);
		IBO_ = 0;
	}

	if (VBO_ != 0)
	{
		glDeleteBuffers(1, &VBO_);
		VBO_ = 0;
	}

	if (VAO_ != 0)
	{
		glDeleteVertexArrays(1, &VAO_);
		VAO_ = 0;
	}

	indexCount_ = 0;
}

/// <summary>
/// �����������Զ��ͷ� GPU ����.
/// </summary>
Mesh::~Mesh()
{
	ClearMesh();
}
