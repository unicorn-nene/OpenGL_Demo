#pragma once

#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"

/// <summary>
/// ��װ��ʹ�� Assimp ���غ���Ⱦ 3D ģ�͵Ĺ��ܡ�
/// Model �ฺ����ļ�����ģ�͡���ȡ����Mesh������ʣ�Texture�����ݣ�
/// ������Ⱦʱ���ݲ��ʰ���Ӧ�����ٻ�������
/// </summary>
class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

	~Model();

private:
	void LoadNode(aiNode *node, const aiScene *scene);
	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	void LoadMaterials(const aiScene *scene);

private:
	std::vector<Mesh*> meshList_{};				   ///< �洢 Model ������ Mesh��ָ��, ÿ�� Mesh ��Ӧ Model һ�������Ĳ���
	std::vector<Texture*> textureList_{};		   ///< �洢��ÿ������������Ӧ�� texture
	std::vector<unsigned int> meshToTex_{};		   ///< ��ÿ�� Mesh ����ӳ�䵽���Ӧ�� Material������
};

