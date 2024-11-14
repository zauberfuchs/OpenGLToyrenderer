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
			//delete texture;
			//texture = nullptr;
		}
	}
}

void Material::SetTexture(Ref<Texture> texture)
{
	m_HasTexture = 1;
	m_Textures.insert({ texture->GetTextureType(), texture });
}

void Material::SetPBRTexture(const std::string& path)
{
	Ref<Texture> tex;

	tex = CreateRef<Texture>(path + "/albedo.png", TextureTarget::Texture2D);
	tex->SetTextureType(TextureType::AlbedoMap);
	tex->SetUniformLocation("material.albedoMap");
	m_Textures.insert({ tex->GetTextureType(), tex });

	tex = CreateRef<Texture>(path + "/normal.png", TextureTarget::Texture2D);
	tex->SetTextureType(TextureType::NormalMap);
	tex->SetUniformLocation("material.normalMap");
	m_Textures.insert({ tex->GetTextureType(), tex });

	tex = CreateRef<Texture>(path + "/metallic.png", TextureTarget::Texture2D);
	tex->SetTextureType(TextureType::MetallicMap);
	tex->SetUniformLocation("material.metallicMap");
	m_Textures.insert({ tex->GetTextureType(), tex });

	tex = CreateRef<Texture>(path + "/roughness.png", TextureTarget::Texture2D);
	tex->SetTextureType(TextureType::RoughnessMap);
	tex->SetUniformLocation("material.roughnessMap");
	m_Textures.insert({ tex->GetTextureType(), tex });

	tex = CreateRef<Texture>(path + "/ao.png", TextureTarget::Texture2D);
	tex->SetTextureType(TextureType::AmbientOcclusionMap);
	tex->SetUniformLocation("material.aoMap");
	m_Textures.insert({ tex->GetTextureType(), tex });
}

Ref<Texture> Material::GetTexture(const TextureType& channelMap) const
{
	return m_Textures.at(channelMap);
}

void Material::SetReflectionProbe(Ref<ReflectionProbe> probe)
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
	Ref<Scene> activeScene = World::Get().GetActiveScene();
	auto lights = World::Get().GetEntityManager()->GetActiveComponents<Light>();
	
	switch(m_Type)
	{
	case MaterialType::Phong :
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

	//for (int i = 0; i < lights.size(); i++)
	int i = 0;
	for(auto& l : lights)
	{
		EntityID eID = lights.begin()->first;
		Transform& t = World::Get().GetEntityManager()->GetComponent<Transform>(eID);
		m_Shader->SetUniform3f("light[" + std::to_string(i) + "].position", t.GetLocalPosition());
		m_Shader->SetUniform3f("light[" + std::to_string(i) + "].color", l.second->GetColor());
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
		auto m = CreateRef<Material>(entry.path().filename().stem().string());
		m->SetType(MaterialType::TexturedPhysicallyBased);
		m->SetPBRTexture(entry.path().string());
		World::Get().AddMaterial(m);
	}
}