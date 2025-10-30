#include "Model.h"

/// <summary>
/// Ĭ�Ϲ��캯��
/// </summary>
Model::Model()
{
}

/// <summary>
/// ʹ�� Assimp ����ģ���ļ�, �ú���������ļ��ṹ���Զ���������Ͳ�����Դ��
/// </summary>
/// <param name="fileName">model�ļ�·��</param>
void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Model (%s) failed to load: %s", fileName.c_str(), importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);
}

/// <summary>
/// ��Ⱦģ�������е����񲿷֡�
/// ����ÿ������Ĳ��������󶨶�Ӧ�������� Mesh::RenderMesh() ���ơ�
/// </summary>
void Model::RenderModel()
{
	for (size_t i = 0; i < meshList_.size(); i++)
	{
		unsigned int materialIndex = meshToTex_[i];

		if (materialIndex < textureList_.size() && textureList_[materialIndex])
		{
			textureList_[materialIndex]->UseTexture();
		}

		meshList_[i]->RenderMesh();
	}
}


/// <summary>
/// ������Դ
/// </summary>
void Model::ClearModel()
{
	for (size_t i = 0; i < meshList_.size(); i++)
	{
		if (meshList_[i])
		{
			delete meshList_[i];
			meshList_[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList_.size(); i++)
	{
		if (textureList_[i])
		{
			delete textureList_[i];
			textureList_[i] = nullptr;
		}
	}
}

/// <summary>
/// ��������
/// </summary>
Model::~Model()
{
	ClearModel();
}


///============================= private function =====================================

/// <summary>
/// �ݹ���� aiNode�Ľڵ㼰���ӽڵ�
/// </summary>
/// <param name="node">Ҫ���ص� aiNodeָ��</param>
/// <param name="scene">���� Node�� aiSceneָ��</param>
void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	// ���ص�ǰ�ڵ����������
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	// �ݹ���������ӽڵ�
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

/// <summary>
/// �� aiMesh ��������ȡ���㡢���ߡ������������������ݣ���������Ӧ�� Mesh ʵ��
/// </summary>
/// <param name="mesh">Ҫ���ص� aiMesh ָ��</param>
/// <param name="scene">���� Mesh�� aiScene ����ָ��</param>
void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	// ����������
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		// ���� (x, y, z)
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		// ��������(u, v)
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		//�������� (nx, ny, nz)
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	// ��������
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// �����µ� Mesh ʵ��
	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList_.push_back(newMesh);
	meshToTex_.push_back(mesh->mMaterialIndex);
}

/// <summary>
/// ���� Model�����в���(material)
/// ��ģ��δ�����������Զ�ʹ��Ĭ�ϵ� plain.png��
/// </summary>
/// <param name="scene">Assimp ����� aiScene ����</param>
void Model::LoadMaterials(const aiScene* scene)
{
	textureList_.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList_[i] = nullptr;

		// ��� material ����, ������������ͼ(diffuse)
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// ��ȡ�ļ���
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);
				// ��������·��
				std::string texPath = std::string("Textures/") + filename;

				textureList_[i] = new Texture(texPath.c_str());
				if (!textureList_[i]->LoadTexture())
				{
					printf("Failed to load texture at: %s\n", texPath.c_str());
					delete textureList_[i];
					textureList_[i] = nullptr;
				}
			}
		}

		// ������������, ��ʹ��Ĭ������
		if (!textureList_[i])
		{
			textureList_[i] = new Texture("Textures/plain.png");
			textureList_[i]->LoadTextureA();
		}
	}
}
