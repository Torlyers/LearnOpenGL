#include "Mesh.h"



Mesh::Mesh()
{
}

Mesh::Mesh(vector<Vertex> & i_Vertices, vector<unsigned int> & i_Indices, vector<Texture> & i_Textures)	
{
	m_Vertices = i_Vertices;
	m_Indices = i_Indices;
	m_Textures = i_Textures;

	Init();
}

Mesh::~Mesh()
{
}

void Mesh::Init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_Normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_TexCoords)));

	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_Tangent)));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_Bitangent)));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader i_Shader)
{
	unsigned int DiffuseIndex  = 1;
	unsigned int SpecularIndex = 1;

	for (int i = 0; i < m_Textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		string s = "";
		switch (m_Textures[i].m_Type)
		{
		case DIFFUSE:
			s = "texture_diffuse" + std::to_string(i + 1);
			break;
		case SPECULAR:
			s = "texture_specular" + std::to_string(i + 1);
			break;
		case NORMAL:
			s = "texture_normal" + std::to_string(i + 1);
			break;
		case HEIGHT:
			s = "texture_height" + std::to_string(i + 1);
			break;
		default:
			break;
		}

		glUniform1i(glGetUniformLocation(i_Shader.GetID(), s.c_str()), i);
		//i_Shader.setInt(s.c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].m_ID);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}


