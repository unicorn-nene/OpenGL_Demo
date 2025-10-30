#include "Skybox.h"


/// <summary>
/// ���캯��
/// </summary>
Skybox::Skybox()
{
}

/// <summary>
/// ���캯��: 
/// ʹ�ø�����������������ͼ������պУ�����ʼ����������ɫ��
/// </summary>
/// <param name="faceLocation">������������ͼ��·���� vector</param>
Skybox::Skybox(std::vector<std::string> faceLocations)
{
	// ��ɫ����ʼ��
	skyShader_ = new Shader();
	skyShader_->CreateFromFiles("Shaders/skybox.vert", "Shaders/skybox.frag");

	// �� Shader �л�ȡ uniform ����λ��
	uniformProjection_ = skyShader_->GetProjectionLocation();
	uniformView_ = skyShader_->GetViewLocation();

	// ������������ͼ����
	glGenTextures(1, &textureId_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);

	int width, height, bitDepth;

	// ���� 6����������ļ�,���ص���������ͼ����
	for (size_t i = 0; i < 6; ++i)
	{
		unsigned char* texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
		if (!texData)
		{
			printf("Failed to find: %s\n", faceLocations[i].c_str());
			return;
		}

		// ��ͼ�������ϴ�����������ͼ�Ķ�Ӧ��
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

		stbi_image_free(texData);
	}

	// ������������ͼ���������
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ���� skyBox ������ Mesh

	// ������������
	unsigned int skyboxIndices[] = {
		// ǰ��
		0, 1, 2,
		2, 1, 3,
		// ����
		2, 3, 5,
		5, 3, 7,
		// ����
		5, 7, 4,
		4, 7, 6,
		// ����
		4, 6, 0,
		0, 6, 1,
		// ����
		4, 0, 5,
		5, 0, 2,
		// ����
		1, 6, 3,
		3, 6, 7
	};

	// �������꣨ÿ������ 8 �� float��������ֻ�õ�λ�����꣩
	float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f,  1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	skyMesh_ = new Mesh();
	skyMesh_->CreateMesh(skyboxVertices, skyboxIndices, 64, 36);
}


/// <summary>
/// �ڵ�ǰ OpenGL �����л�����պ�
/// �����д����ʱ�رգ�ʹ�� view �� projection ������������屳��
/// </summary>
/// <param name="viewMatrix">��ǰ���������ͼ����</param>
/// <param name="projectionMatrix">��ǰʹ�õ�ͶӰ����</param>
void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	// �Ƴ���ͼ�����е�ƽ�Ʋ���, ����պ�ʼ�ո��������
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	// ��ֹд����Ȼ�����(��ֹ��պ��ڵ�����)
	glDepthMask(GL_FALSE);

	skyShader_->UseShader();

	glUniformMatrix4fv(uniformProjection_, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView_, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);

	skyShader_->Validate();

	skyMesh_->RenderMesh();

	glDepthMask(GL_TRUE);
}


/// <summary>
/// ���캯��
/// </summary>
Skybox::~Skybox()
{
}
