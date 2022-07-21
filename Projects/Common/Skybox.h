#pragma once

#include "ISceneObject.h"
#include "Shader.h"
#include <stb_image.h>

class Skybox
{
public:
	Skybox(const std::string& name);
	~Skybox() = default;

	void Render();

private:
	std::string m_Name;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_Components;

	Shader* m_Shader;

	GLuint m_ID;
	GLuint m_SkyboxVAO, m_SkyboxVBO, m_SkyboxEBO;

	std::vector<std::string> m_Faces
	{
			"../Data/Textures/Skybox/right.png",
			"../Data/Textures/Skybox/left.png",
			"../Data/Textures/Skybox/top.png",
			"../Data/Textures/Skybox/bottom.png",
			"../Data/Textures/Skybox/front.png",
			"../Data/Textures/Skybox/back.png"
	};
	
	void Load();
	void SetupSkybox();
};