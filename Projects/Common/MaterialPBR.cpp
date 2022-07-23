#include "MaterialPBR.h"
#include "IMesh.h"
#include "Shader.h"
#include "World.h"

MaterialPBR::MaterialPBR()
	: m_Name("default"),
	m_Color(1.0f),
	m_Albedo(0.0f),
	m_Metallic(0.0f),
	m_Roughness(0.0f),
	m_Ao(0.0f)
{
}

MaterialPBR::MaterialPBR(const std::string& name)
	: m_Name(name),
	m_Color(1.0f),
	m_Albedo(0.0f),
	m_Metallic(0.0f),
	m_Roughness(0.0f),
	m_Ao(0.0f)
{
}

MaterialPBR::~MaterialPBR()
{
}

void MaterialPBR::SetTexture(ITexture* texture)
{

	switch (texture->GetTextureType()) {
	case ETextureChannels::AlbedoMap:
		m_TextureAlbedo = texture;
		break;
	case ETextureChannels::NormalMap:
		m_TextureNormal = texture;
		break;
	case ETextureChannels::MetallicMap:
		m_TextureMetallic = texture;
		break;
	case ETextureChannels::RoughnessMap:
		m_TextureRoughness = texture;
		break;
	case ETextureChannels::AmbientOcclusionMap:
		m_TextureAmbienOcclusion = texture;
		break;
	default:
		break;
	}

}

ITexture* MaterialPBR::GetTexture(ETextureChannels channelMap)
{
	ITexture* retTexture;

	switch (channelMap) {
	case ETextureChannels::AlbedoMap:
		retTexture = m_TextureAlbedo;
		break;
	case ETextureChannels::NormalMap:
		retTexture = m_TextureNormal;
		break;
	case ETextureChannels::MetallicMap:
		retTexture = m_TextureMetallic;
		break;
	case ETextureChannels::RoughnessMap:
		retTexture = m_TextureRoughness;
		break;
	case ETextureChannels::AmbientOcclusionMap:
		retTexture = m_TextureAmbienOcclusion;
		break;
	default:
		break;
	}

	return retTexture;
}

void MaterialPBR::SetShader(IShader* shader)
{
	m_Shader = shader;
}

IShader* MaterialPBR::GetShader()
{
	return m_Shader;
}

void MaterialPBR::SetName(const std::string name)
{
	m_Name = name;
}

std::string MaterialPBR::GetName()
{
	return m_Name;
}

void MaterialPBR::SetReflectionProbe(ReflectionProbe* probe)
{
	m_Probe = probe;
	m_TextureBrdfLookUp = m_Probe->GetBrdfLookUpTexture();
	m_TexturePrefilter = m_Probe->GetPrefilterTexture();
	m_TextureIrradiance = m_Probe->GetIrradianceTexture();
}


void MaterialPBR::RenderPre()
{
	if (m_Shader != nullptr) {
		m_Shader->Bind();
	}

	if (m_TextureAlbedo != nullptr) {
		m_TextureAlbedo->RenderPre();
		m_Shader->SetUniform1i("material.albedoMap", 0);
	}
	if (m_TextureNormal != nullptr) {
		m_TextureNormal->RenderPre();
		m_Shader->SetUniform1i("material.normalMap", 1);
	}
	if (m_TextureMetallic != nullptr) {
		m_TextureMetallic->RenderPre();
		m_Shader->SetUniform1i("material.metallicMap", 2);
	}
	if (m_TextureRoughness != nullptr) {
		m_TextureRoughness->RenderPre();
		m_Shader->SetUniform1i("material.roughnessMap", 3);
	}
	if (m_TextureAmbienOcclusion != nullptr) {
		m_TextureAmbienOcclusion->RenderPre();
		m_Shader->SetUniform1i("material.aoMap", 4);
	}
	if (m_TextureIrradiance != nullptr) {
		m_TextureIrradiance->RenderPre();
		m_Shader->SetUniform1i("material.irradianceMap", 5);
	}
	if (m_TexturePrefilter != nullptr) {
		m_TexturePrefilter->RenderPre();
		m_Shader->SetUniform1i("material.prefilterMap", 6);
	}
	if (m_TextureBrdfLookUp != nullptr) {
		m_TextureBrdfLookUp->RenderPre();
		m_Shader->SetUniform1i("material.brdfLUT", 7);
	}

}

void MaterialPBR::Render()
{
	RenderPre();
	Scene* activeScene = World::Get().GetActiveScene();
	Camera* sceneCamera = activeScene->GetSceneCamera();
	sceneCamera->UpdateMatrix(m_Shader);
	auto lights = activeScene->GetSceneLightSources();

	if(m_TextureAlbedo == nullptr)
	{
		m_Shader->SetUniform3f("material.color", m_Color);
		m_Shader->SetUniform3f("material.albedo", m_Albedo);
		m_Shader->SetUniform1f("material.metallic", m_Metallic);
		m_Shader->SetUniform1f("material.roughness", m_Roughness);
		m_Shader->SetUniform1f("material.ao", m_Ao);
	}

	
	m_Shader->SetUniform3f("camPos", sceneCamera->Position);

	m_Shader->SetUniform1i("numPointlights", (int)lights.size());
	for (auto& l : lights)
	{
		int i = 0;
		Light* light = l.second;
		switch (light->Type)
		{
		case LightSourceType::PointLight:
			m_Shader->SetUniform3f("light[0].position", light->Position);
			m_Shader->SetUniform3f("light[0].color", light->Color);
			m_Shader->SetUniform3f("pointLights[" + std::to_string(i) + "].position", light->Position);
			m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].constant", light->Constant);
			m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].linear", light->Linear);
			m_Shader->SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", light->Quadratic);
			break;

		case LightSourceType::DirectionalLight:
			m_Shader->SetUniform3f("light.Position", light->Position);
			m_Shader->SetUniform3f("light.Color", light->Color);
			m_Shader->SetUniform3f("dirLight.direction", light->Direction);
			break;
		case LightSourceType::SpotLight:
			break;
		}
		i++;
	}
}

void MaterialPBR::RenderPost()
{
	/*if (m_TextureDiff != nullptr) {
		m_TextureDiff->Unbind();
	}

	if (m_TextureSpec != nullptr) {
		m_TextureSpec->Unbind();
	}*/

	if (m_Shader != nullptr) {
		m_Shader->Unbind();
	}
}
