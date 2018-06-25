#include "Model.h"
#include "File.h"


Model::Model()
{
}

Model::Model(const char * path) :
	m_FilePath(path)
{
	Init();
}


Model::~Model()
{
}

void Model::Init()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}

	m_Directory = m_FilePath.substr(0, m_FilePath.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::Draw(const Shader & i_Shader)
{
	for (auto mesh : m_Meshes)
		mesh.Draw(i_Shader);
}

void Model::ProcessNode(aiNode * i_Node, const aiScene * i_Scene)
{
	for (int i = 0; i < i_Node->mNumMeshes; ++i)
	{
		aiMesh* mesh = i_Scene->mMeshes[i_Node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, i_Scene));
	}

	for (int i = 0; i < i_Node->mNumChildren; ++i)
	{
		ProcessNode(i_Node->mChildren[i], i_Scene);
	}
}

Mesh Model::ProcessMesh(aiMesh * i_Mesh, const aiScene * i_Scene)
{
	vector<Vertex>         vertices;
	vector<Texture>        textures;
	vector<unsigned int>   indices;

	for (int i = 0; i < i_Mesh->mNumVertices; ++i)
	{
		Vertex vertex;

		vertex.m_Position.x = i_Mesh->mVertices[i].x;
		vertex.m_Position.y = i_Mesh->mVertices[i].y;
		vertex.m_Position.z = i_Mesh->mVertices[i].z;

		vertex.m_Normal.x = i_Mesh->mNormals[i].x;
		vertex.m_Normal.y = i_Mesh->mNormals[i].y;
		vertex.m_Normal.z = i_Mesh->mNormals[i].z;

		if (i_Mesh->mTextureCoords[0])
		{
			vertex.m_TexCoords.x = i_Mesh->mTextureCoords[0][i].x;
			vertex.m_TexCoords.y = i_Mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.m_TexCoords = vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (int i = 0; i < i_Mesh->mNumFaces; ++i)
	{
		aiFace face = i_Mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = i_Scene->mMaterials[i_Mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	vector<Texture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE, DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = LoadTextures(material, aiTextureType_SPECULAR, SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = LoadTextures(material, aiTextureType_HEIGHT, NORMAL);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = LoadTextures(material, aiTextureType_AMBIENT, HEIGHT);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);

}

vector<Texture> Model::LoadTextures(aiMaterial * i_Mat, aiTextureType i_Type, TextureType i_TypeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < i_Mat->GetTextureCount(i_Type); i++)
	{
		aiString str;
		i_Mat->GetTexture(i_Type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].m_Path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.m_ID = TextureFromFile(str.C_Str(), m_Directory, false);
			texture.m_Type = i_TypeName;
			texture.m_Path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}
