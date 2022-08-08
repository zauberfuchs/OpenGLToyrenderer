#include "../../Engine/Utils/pch.h"
#include "Material.h"

#include "../../Engine/OpenGL/Shader.h"

Material::Material()
	: m_Name("default"),
	m_Color(1.0f),
	m_Albedo(0.0f),
	m_Metallic(0.0f),
	m_Roughness(0.0f),
	m_Ao(0.0f)
{
}

Material::Material(const std::string& name)
	: m_Name(name),
	m_Color(1.0f),
	m_Albedo(0.0f),
	m_Metallic(0.0f),
	m_Roughness(0.0f),
	m_Ao(0.0f)
{
}

Material::~Material()
{
}

void Material::SetTexture(ITexture* texture)
{
	m_Textures.insert({ texture->GetTextureType(), texture });
}

void Material::SetPBRTexture(const std::string& path)
{
	//Todo name hinzufügen?
	m_TextureAlbedo = new Texture(path + "/albedo.png", TextureType::AlbedoMap);
	m_TextureAlbedo->SetUniformLocation("material.albedoMap");
	m_TextureAlbedo->SetTextureTarget(TextureTarget::Texture2D);

	m_TextureMetallic = new Texture(path + "/metallic.png", TextureType::MetallicMap);
	m_TextureMetallic->SetUniformLocation("material.metallicMap");
	m_TextureMetallic->SetTextureTarget(TextureTarget::Texture2D);

	m_TextureNormal = new Texture(path + "/normal.png", TextureType::NormalMap);
	m_TextureNormal->SetUniformLocation("material.normalMap");
	m_TextureNormal->SetTextureTarget(TextureTarget::Texture2D);

	m_TextureRoughness = new Texture(path + "/roughness.png", TextureType::RoughnessMap);
	m_TextureRoughness->SetUniformLocation("material.roughnessMap");
	m_TextureRoughness->SetTextureTarget(TextureTarget::Texture2D);

	m_TextureAmbienOcclusion = new Texture(path + "/ao.png", TextureType::AmbientOcclusionMap);
	m_TextureAmbienOcclusion->SetUniformLocation("material.aoMap");
	m_TextureAmbienOcclusion->SetTextureTarget(TextureTarget::Texture2D);

	m_Textures.insert({ m_TextureAlbedo->GetTextureType(), m_TextureAlbedo });
	m_Textures.insert({ m_TextureMetallic->GetTextureType(), m_TextureMetallic });
	m_Textures.insert({ m_TextureNormal->GetTextureType(), m_TextureNormal });
	m_Textures.insert({ m_TextureRoughness->GetTextureType(), m_TextureRoughness });
	m_Textures.insert({ m_TextureAmbienOcclusion->GetTextureType(), m_TextureAmbienOcclusion });
}

ITexture* Material::GetTexture(const TextureType& channelMap)
{
	return m_Textures.at(channelMap);
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

void Material::SetReflectionProbe(ReflectionProbe* probe)
{
	m_Probe = probe;

	m_Textures.insert({ m_Probe->GetBrdfLookUpTexture()->GetTextureType() ,m_Probe->GetBrdfLookUpTexture() });
	m_Textures.insert({ m_Probe->GetPrefilterTexture()->GetTextureType() ,m_Probe->GetPrefilterTexture() });
	m_Textures.insert({ m_Probe->GetIrradianceTexture()->GetTextureType() ,m_Probe->GetIrradianceTexture() });

	m_TextureBrdfLookUp = m_Probe->GetBrdfLookUpTexture();
	m_TexturePrefilter = m_Probe->GetPrefilterTexture();
	m_TextureIrradiance = m_Probe->GetIrradianceTexture();
}
//Todo ergibt der name sinn?
void Material::SetType(const MaterialType& type)
{
	switch(type)
	{
	case MaterialType::Phong:
		m_Type = type;
		m_Shader = World::Get().GetShader("phong");
		break;
	case MaterialType::PhysicallyBased:
		m_Type = type;
		m_Shader = World::Get().GetShader("pbrMaterial");
		break;
	case MaterialType::TexturedPhysicallyBased:
		m_Type = type;
		m_Shader = World::Get().GetShader("pbrMaterialTextured");
		break;
	}
}

void Material::SetupTextures()
{

	if (m_Shader != nullptr) {
		m_Shader->Bind();
	}
	
	int i = 0;
	for(const auto& [texturetType, texture] : m_Textures)
	{
		texture->Bind(i, texture->GetTextureTarget());
		m_Shader->SetUniform1i(texture->GetUniformLocation(), i);
		i++;
	}

}

void Material::SetupUniforms()
{
	SetupTextures();

	Scene* activeScene = World::Get().GetActiveScene();
	Camera* sceneCamera = activeScene->GetSceneCamera();
	sceneCamera->UpdateMatrix(m_Shader);
	auto lights = activeScene->GetSceneLightSources();

	m_Shader->SetUniform3f("camPos", sceneCamera->Position);

	switch(m_Type)
	{
	case MaterialType::Phong :
		m_Shader->SetUniform1i("hasAmbient", m_HasAmbient);
		m_Shader->SetUniform1i("hasDiffuse", m_HasDiffuse);
		m_Shader->SetUniform1i("hasSpecular", m_HasSpecular);
		m_Shader->SetUniform1i("hasTexture", m_HasTexture);

		m_Shader->SetUniform3f("material.ambient", m_Ambient);
		m_Shader->SetUniform3f("material.diffuse", m_Diffuse);
		m_Shader->SetUniform3f("material.specular", m_Specular);
		m_Shader->SetUniform1f("material.shininess", m_Shininess);
		m_Shader->SetUniform3f("material.color", m_Color);
		break;
	case MaterialType::PhysicallyBased :
		m_Shader->SetUniform3f("material.albedo", m_Albedo);
		m_Shader->SetUniform1f("material.metallic", m_Metallic);
		m_Shader->SetUniform1f("material.roughness", m_Roughness);
		m_Shader->SetUniform1f("material.ao", m_Ao);
		break;
	case MaterialType::TexturedPhysicallyBased :
		break;
	}

	int i = 0;
	for (auto& l : lights)
	{
		Light* light = l.second;
		
		m_Shader->SetUniform3f("light[" + std::to_string(i) + "].position", light->GetPosition());
		m_Shader->SetUniform3f("light[" + std::to_string(i) + "].color", light->GetColor());
		i++;
	}
}

void Material::UnbindTextures()
{
	for (const auto& [textureType, texture] : m_Textures)
	{
		texture->Unbind();
	}
	if (m_Shader != nullptr) {
		m_Shader->Unbind();
	}
}

void MaterialLoader::LoadMaterialFolder(const std::string& path)
{
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		Material* m = new Material(entry.path().filename().stem().u8string());
		m->SetType(MaterialType::TexturedPhysicallyBased);
		m->SetPBRTexture(entry.path().u8string());
		m->SetReflectionProbe(World::Get().GetActiveScene()->GetReflectionProbe());
		World::Get().AddMaterial(m);
	}
}

