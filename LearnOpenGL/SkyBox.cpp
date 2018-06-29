#include "SkyBox.h"
#include "Settings.h"

SkyBox* SkyBox::m_Instance = nullptr;

SkyBox* SkyBox::Instance()
{
	if (m_Instance == nullptr)
		m_Instance = new SkyBox();

	return m_Instance;
}


SkyBox::SkyBox()
{
	m_FacePaths = 
	{
		SKYBOX_RIGHT,
		SKYBOX_LEFT,
		SKYBOX_TOP,
		SKYBOX_BOTTOM,
		SKYBOX_BACK,
		SKYBOX_FRONT		
	};

	m_CubeTexture = loadCubemap(m_FacePaths);

	Init();
}

SkyBox::~SkyBox()
{
}

void SkyBox::Init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);	

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void SkyBox::Draw(Shader i_Shader)
{
	i_Shader.use();	
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTexture);	
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}


