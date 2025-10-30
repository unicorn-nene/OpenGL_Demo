#pragma once

#include <vector>
#include <string>

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Mesh.h"
#include "Shader.h"

/// <summary>
/// Skybox �������� 3D �����д�������Ⱦ��պУ���ձ��������壩.
/// ���������������ͼ����������ͼ������ʹ��һ���������ɫ���ڳ��������л���
/// </summary>
class Skybox
{
public:
	Skybox();

	Skybox(std::vector<std::string> faceLocations);

	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	~Skybox();

private:
	Mesh* skyMesh_{};					 ///< ��պ� Mesh
	Shader* skyShader_{};				 ///< ��պ� Shader

	GLuint textureId_{};				 ///< ���� ID
	GLuint uniformProjection_{};		 ///< ͶӰ����
	GLuint uniformView_{};				 ///< ���������ͼ����
};

