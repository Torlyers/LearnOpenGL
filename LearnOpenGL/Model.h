#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include<string>
#include<vector>
#include "Mesh.h"

#include"glm/glm.hpp"


using namespace std;
using namespace glm;

class Model
{
public:
	
	Model();
	Model(const char* path);
	Model(const Model & i_other);
	~Model();

	void Init();
	void Draw(const Shader & i_Shader);

private:

	vector<Mesh> m_Meshes;
	vector<Texture> textures_loaded;
	string m_FilePath;
	string m_Directory;

	void ProcessNode(aiNode* i_Node, const aiScene* i_Scene);
	Mesh ProcessMesh(aiMesh* i_Mesh, const aiScene* i_Scene);

	vector<Texture> LoadTextures(aiMaterial* i_Mat, aiTextureType i_Type, TextureType i_TypeName);
};

