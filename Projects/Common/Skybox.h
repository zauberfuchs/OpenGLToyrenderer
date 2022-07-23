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
	inline unsigned int GetId() { return m_ID; }
	inline void SetId(const unsigned int& id) { m_ID = id; }

private:
	std::string m_Name;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_Components;

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
	
	void Load();
	void SetupSkybox();
};





