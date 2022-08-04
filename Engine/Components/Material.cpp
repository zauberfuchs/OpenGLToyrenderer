#include "../../Engine/Utils/pch.h"
#include "Material.h"

#include "../../Engine/OpenGL/Shader.h"
#include "../../Engine/Scene/World.h"

Material::Material()
	: m_Name("default"), m_Color(1.0f)
{
}

Material::Material(const std::string& name)
	: m_Name(name), m_Color(1.0f)
{
}

Material::~Material()
{
}

void Material::SetTexture(ITexture* texture)
{
	m_HasTexture = 1;
	switch (texture->GetTextureType()) {
	case ETextureChannels::AlbedoMap:
		m_TextureAlbedo = texture;
		break;
	default:
		break;
	}

}

ITexture* Material::GetTexture(ETextureChannels channelMap)
{
	ITexture* retTexture;

	switch (channelMap) {
	case ETextureChannels::AlbedoMap:
		retTexture = m_TextureAlbedo;
		break;
	default:
		break;
	}

	return retTexture;
}

void Material::SetShader(IShader* shader)
{
	m_Shader = shader;
}

IShader* Material::GetShader()
{
	return m_Shader;
}

glm::vec3 Material::GetColor()
{
	return m_Color;
}

void Material::SetColor(const glm::vec3& color)
{
	m_Color = color;
}

void Material::SetName(const std::string name)
{
	m_Name = name;
}

std::string Material::GetName()
{
	return m_Name;
}

void Material::SetReflections(ReflectionType r)
{
	switch (r)
	{
	case ReflectionType::Ambient: m_HasAmbient = 1; break;
	case ReflectionType::Diffuse: m_HasDiffuse = 1; break;
	case ReflectionType::Specular: m_HasSpecular = 1; break;
	case ReflectionType::Phong: m_HasDiffuse = 1; m_HasAmbient = 1; m_HasSpecular = 1; break;
	}
}


void Material::RenderPre()
{
	if (m_Shader != nullptr)
		m_Shader->Bind();

	if (m_TextureAlbedo != nullptr) {
		m_TextureAlbedo->RenderPre();
		m_Shader->SetUniform1i("hasTexture", m_HasTexture);
		m_Shader->SetUniform1i("albedoMap", 0);
	}
}

void Material::Render()
{
	RenderPre();

	Scene* activeScene = World::Get().GetActiveScene();
	Camera* sceneCamera = activeScene->GetSceneCamera();
	sceneCamera->UpdateMatrix(m_Shader);
	auto lights = activeScene->GetSceneLightSources();

	m_Shader->SetUniform1i("hasAmbient", m_HasAmbient);
	m_Shader->SetUniform1i("hasDiffuse", m_HasDiffuse);
	m_Shader->SetUniform1i("hasSpecular", m_HasSpecular);
	m_Shader->SetUniform1i("hasTexture", m_HasTexture);

	m_Shader->SetUniform1i("hasDiffuseMap", m_HasDiffuseMap);

	m_Shader->SetUniform3f("material.ambient", Ambient);
	m_Shader->SetUniform3f("material.diffuse", Diffuse);
	m_Shader->SetUniform3f("material.specular", Specular);
	m_Shader->SetUniform1f("material.shininess", Shininess);
	m_Shader->SetUniform3f("material.color", m_Color);


	m_Shader->SetUniform3f("camPos", sceneCamera->Position);

	m_Shader->SetUniform1i("numPointlights", (int)lights.size());

	int i = 0;
	for (auto& l : lights)
	{
		Light* light = l.second;

		switch (light->GetType())
		{
		case LightSourceType::PointLight:
			m_Shader->SetUniform3f("light.position", light->GetPosition());
			m_Shader->SetUniform3f("light.color", light->GetColor());
			m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].position", light->GetPosition());
			m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].constant", light->GetConstant());
			m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].linear", light->GetLinear());
			m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", light->GetQuadratic());
			m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].ambient", Ambient);
			m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].diffuse", Diffuse);
			m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].specular", Specular);
			m_Shader->SetUniform1f("material.shininess", Shininess);
			break;

		case LightSourceType::DirectionalLight:
			m_Shader->SetUniform3f("light.position", light->GetPosition());
			m_Shader->SetUniform3f("light.color", light->GetColor());
			m_Shader->SetUniform3f("dirLight.direction", light->GetDirection());
			m_Shader->SetUniform3f("dirLight.ambient", Ambient);
			m_Shader->SetUniform3f("dirLight.diffuse", Diffuse);
			m_Shader->SetUniform3f("dirLight.specular", Specular);
			m_Shader->SetUniform1f("material.shininess", Shininess);
			break;
		case LightSourceType::SpotLight:
			break;
		}
		i++;
	}
}

void Material::RenderPost()
{
	if (m_TextureAlbedo != nullptr) {
		m_TextureAlbedo->Unbind();
	}

	if (m_Shader != nullptr) {
		m_Shader->Unbind();
	}
}
