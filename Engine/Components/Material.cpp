#include "Engine/Utils/pch.h"
#include "Material.h"

#include "Engine/OpenGL/Shader.h"
#include "Engine/OpenGL/Texture.h"

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
	for (auto [textureType, texture] : m_Textures)
	{
		if(textureType != TextureType::DepthMap)
		{
			//todo assetmanager
//			delete texture;
	//		texture = nullptr;
		}
	}
}

void Material::SetTexture(Texture* texture)
{
	m_HasTexture = 1;
	m_Textures.insert({ texture->GetTextureType(), texture });
}

// mehr parameter?? type/ target / uniform
void Material::SetPBRTexture(const std::string& path)
{
	Texture* tex;

	tex = new Texture(path + "/albedo.png");
	tex->SetTextureType(TextureType::AlbedoMap);
	tex->SetUniformLocation("material.albedoMap");
	tex->SetTextureTarget(TextureTarget::Texture2D);
	m_Textures.insert({ tex->GetTextureType(), tex });

	tex = new Texture(path + "/normal.png");
	tex->SetTextureType(TextureType::NormalMap);
	tex->SetUniformLocation("material.normalMap");
	tex->SetTextureTarget(TextureTarget::Texture2D);
	m_Textures.insert({ tex->GetTextureType(), tex });

	tex = new Texture(path + "/metallic.png");
	tex->SetTextureType(TextureType::MetallicMap);
	tex->SetUniformLocation("material.metallicMap");
	tex->SetTextureTarget(TextureTarget::Texture2D);
	m_Textures.insert({ tex->GetTextureType(), tex });

	tex = new Texture(path + "/roughness.png");
	tex->SetTextureType(TextureType::RoughnessMap);
	tex->SetUniformLocation("material.roughnessMap");
	tex->SetTextureTarget(TextureTarget::Texture2D);
	m_Textures.insert({ tex->GetTextureType(), tex });

	tex = new Texture(path + "/ao.png");
	tex->SetTextureType(TextureType::AmbientOcclusionMap);
	tex->SetUniformLocation("material.aoMap");
	tex->SetTextureTarget(TextureTarget::Texture2D);
	m_Textures.insert({ tex->GetTextureType(), tex });
}

Texture* Material::GetTexture(const TextureType& channelMap)
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
}

void Material::UpdateReflectionProbe()
{
	m_Textures.insert({ m_Probe->GetBrdfLookUpTexture()->GetTextureType() ,m_Probe->GetBrdfLookUpTexture() });
	m_Textures.insert({ m_Probe->GetPrefilterTexture()->GetTextureType() ,m_Probe->GetPrefilterTexture() });
	m_Textures.insert({ m_Probe->GetIrradianceTexture()->GetTextureType() ,m_Probe->GetIrradianceTexture() });
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
	if(m_Probe != nullptr)
	{
		UpdateReflectionProbe();
	}

	if (m_Shader != nullptr) {
		m_Shader->Bind();
	}
	
	int i = 0;
	for(const auto& [texturetType, texture] : m_Textures)
	{
		texture->Bind(i);
		m_Shader->SetUniform1i(texture->GetUniformLocation(), i);
		i++;
	}

}

void Material::SetupUniforms()
{
	Scene* activeScene = World::Get().GetActiveScene();
	auto lights = activeScene->GetSceneLightSources();

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

		if(m_HasTexture)
		{
			SetupTextures();
		}
		break;
	case MaterialType::PhysicallyBased :
		m_Shader->SetUniform3f("material.albedo", m_Albedo);
		m_Shader->SetUniform1f("material.metallic", m_Metallic);
		m_Shader->SetUniform1f("material.roughness", m_Roughness);
		m_Shader->SetUniform1f("material.ao", m_Ao);
		break;
	case MaterialType::TexturedPhysicallyBased :
		SetupTextures();
		break;
	}

	//todo light pass
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
		auto* m = new Material(entry.path().filename().stem().u8string());
		m->SetType(MaterialType::TexturedPhysicallyBased);
		m->SetPBRTexture(entry.path().u8string());
		World::Get().AddMaterial(m);
	}
}