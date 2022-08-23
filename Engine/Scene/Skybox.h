#pragma once

#include "Engine/OpenGL/Shader.h"
#include "Engine/OpenGL/Texture.h"

class Skybox
{
public:
	Skybox(const std::string& name);
	~Skybox() = default;
	
	inline void SetCubeMapTexture(Texture* texture) { m_Skybox = texture; }
	inline Texture* GetCubeMapTexture() { return m_Skybox; }

private:
	std::string m_Name;

	Texture* m_Skybox;
};





