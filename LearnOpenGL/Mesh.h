#pragma once

#include<string>
#include<vector>

#include"glm/glm.hpp"
#include"Shader.h"

using namespace glm;
using namespace std;

struct Vertex
{
	vec3 m_Position;
	vec3 m_Normal;
	vec2 m_TexCoords;
};

enum TextureType
{
	DIFFUSE,
	SPECULAR,
	NORMAL,
	HEIGHT
};

struct Texture
{
	unsigned int  m_ID;
	TextureType   m_Type;
	string        m_Path;
};

class Mesh
{
public:
	Mesh();
	Mesh(vector<Vertex> & i_Vertices, vector<unsigned int> & i_Indices, vector<Texture> & i_Textures);
	~Mesh();

	void Init();
	void Draw(Shader i_Shader);

private:

	vector<Vertex>       m_Vertices;
	vector<unsigned int> m_Indices;
	vector<Texture>      m_Textures;

	unsigned int VAO, VBO, EBO;
};

