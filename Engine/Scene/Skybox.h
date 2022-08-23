#pragma once

#include "Engine/OpenGL/Shader.h"
#include "Engine/OpenGL/Texture.h"

class Skybox
{
public:
	Skybox(const std::string& name);
	~Skybox() = default;

	void Render();
	inline void SetCubeMapTexture(const Texture& texture) { m_Skybox.~Texture(); m_Skybox = texture; }
	inline Texture* GetCubeMapTexture() { return &m_Skybox; }

private:
	std::string m_Name;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_Components;

	Texture m_Skybox;

	Shader* m_Shader;

	GLuint m_ID;
	GLuint m_SkyboxVAO, m_SkyboxVBO, m_SkyboxEBO;

	std::vector<std::string> m_Faces
	{
			"../Data/Textures/Skybox/darkish/right.png",
			"../Data/Textures/Skybox/darkish/left.png",
			"../Data/Textures/Skybox/darkish/top.png",
			"../Data/Textures/Skybox/darkish/bottom.png",
			"../Data/Textures/Skybox/darkish/front.png",
			"../Data/Textures/Skybox/darkish/back.png"
	};

	void SetupSkybox();
};





