#include "Engine/Utils/pch.h"
#include "Skybox.h"

#include "World.h"


Skybox::Skybox(const std::string& name)
	: m_Name(name), m_Skybox(TextureTarget::TextureCubeMap)
{
	m_Shader = World::Get().GetShader("skybox");

	m_Skybox.LoadCubemap("../Data/Textures/Skybox/darkish");
	SetupSkybox();
}

void Skybox::SetupSkybox()
{
	float skyboxVertices[24] =
	{
		//   Coordinates
		-1.0f, -1.0f,  1.0f,//        7--------6
		 1.0f, -1.0f,  1.0f,//       /|       /|
		 1.0f, -1.0f, -1.0f,//      4--------5 |
		-1.0f, -1.0f, -1.0f,//      | |      | |
		-1.0f,  1.0f,  1.0f,//      | 3------|-2
		 1.0f,  1.0f,  1.0f,//      |/       |/
		 1.0f,  1.0f, -1.0f,//      0--------1
		-1.0f,  1.0f, -1.0f
	};

	unsigned int skyboxIndices[] =
	{
		// Right
		6, 2, 1,
		1, 5, 6,
		// Left
		7, 4, 0,
		0, 3, 7,
		// Top
		6, 5, 4,
		4, 7, 6,
		// Bottom
		2, 3, 0,
		0, 1, 2,
		// Back
		5, 1, 0,
		0, 4, 5,
		// Front
		6, 7, 3,
		3, 2, 6
	};

	glGenVertexArrays(1, &m_SkyboxVAO);
	glGenBuffers(1, &m_SkyboxVBO);
	glGenBuffers(1, &m_SkyboxEBO);
	glBindVertexArray(m_SkyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_SkyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SkyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Skybox::Render()
{
	glDepthFunc(GL_LEQUAL);
	Camera* camera = World::Get().GetActiveScene()->GetSceneCamera();

	m_Shader->Bind();
	glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
	
	m_Shader->SetUniformMat4f("view", view);
	m_Shader->SetUniformMat4f("projection", camera->GetProjectionMatrix());
	glBindVertexArray(m_SkyboxVAO);
	m_Skybox.Bind(0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}
