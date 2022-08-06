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

	switch (texture->GetTextureType()) {
	case TextureType::AlbedoMap:
		m_TextureAlbedo = texture;
		break;
	case TextureType::NormalMap:
		m_TextureNormal = texture;
		break;
	case TextureType::MetallicMap:
		m_TextureMetallic = texture;
		break;
	case TextureType::RoughnessMap:
		m_TextureRoughness = texture;
		break;
	case TextureType::AmbientOcclusionMap:
		m_TextureAmbienOcclusion = texture;
		break;
	case TextureType::DepthMap:
		m_TextureShadowDepth = texture;
		break;
	default:
		break;
	}
}

void Material::SetPBRTexture(const std::string& path)
{
	m_TextureAlbedo = new Texture(path + "/albedo.png", TextureType::AlbedoMap);
	m_TextureMetallic = new Texture(path + "/metallic.png", TextureType::MetallicMap);
	m_TextureNormal = new Texture(path + "/normal.png", TextureType::NormalMap);
	m_TextureRoughness = new Texture(path + "/roughness.png", TextureType::RoughnessMap);
	m_TextureAmbienOcclusion = new Texture(path + "/ao.png", TextureType::AmbientOcclusionMap);
}

ITexture* Material::GetTexture(TextureType channelMap)
{
	ITexture* retTexture;

	switch (channelMap) {
	case TextureType::AlbedoMap:
		retTexture = m_TextureAlbedo;
		break;
	case TextureType::NormalMap:
		retTexture = m_TextureNormal;
		break;
	case TextureType::MetallicMap:
		retTexture = m_TextureMetallic;
		break;
	case TextureType::RoughnessMap:
		retTexture = m_TextureRoughness;
		break;
	case TextureType::AmbientOcclusionMap:
		retTexture = m_TextureAmbienOcclusion;
		break;
	default:
		break;
	}

	return retTexture;
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
	m_TextureBrdfLookUp = m_Probe->GetBrdfLookUpTexture();
	m_TexturePrefilter = m_Probe->GetPrefilterTexture();
	m_TextureIrradiance = m_Probe->GetIrradianceTexture();
}

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
	if (m_TextureShadowDepth != nullptr) {
		m_TextureShadowDepth->RenderPre();
		m_Shader->SetUniform1i("depthMap", 8);
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

	//Todo: map von texturen??
	if (m_TextureAlbedo != nullptr) {
		m_TextureAlbedo->Unbind();
	}
	if (m_TextureMetallic != nullptr) {
		m_TextureMetallic->Unbind();
	}
	if (m_TextureNormal != nullptr) {
		m_TextureNormal->Unbind();
	}
	if (m_TextureRoughness != nullptr) {
		m_TextureRoughness->Unbind();
	}
	if (m_TextureAmbienOcclusion != nullptr) {
		m_TextureAmbienOcclusion->Unbind();
	}
	if (m_TexturePrefilter != nullptr) {
		m_TexturePrefilter->Unbind();
	}
	if (m_TextureIrradiance != nullptr) {
		m_TextureIrradiance->Unbind();
	}
	if (m_TextureBrdfLookUp != nullptr) {
		m_TextureBrdfLookUp->Unbind();
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

