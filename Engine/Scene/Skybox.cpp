#include "Engine/Utils/pch.h"
#include "Skybox.h"

#include "Renderer.h"
#include "World.h"


Skybox::Skybox(const std::string& name)
	: m_Name(name), m_Skybox(TextureTarget::TextureCubeMap)
{
	m_Skybox.LoadCubemap("../Data/Textures/Skybox/darkish");
}