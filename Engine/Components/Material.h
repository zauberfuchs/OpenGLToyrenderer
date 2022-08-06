#pragma once

#include "../../Engine/Interfaces/IMaterial.h"
#include "../../Engine/Scene/ReflectionProbe.h"

#include "../../Engine/Scene/World.h"

enum class MaterialType
{
	PhysicallyBased,
	Phong,
	TexturedPhysicallyBased
};



class Material : public IMaterial
{
public:
	Material();
	Material(const std::string& name);

	~Material() override;
	

	void SetTexture(ITexture* texture) override;
	ITexture* GetTexture(TextureType channelMap) override;
	void SetPBRTexture(const std::string& path);

	void SetReflectionProbe(ReflectionProbe* probe);

	void SetupTextures() override;
	void SetupUniforms() override;
	void UnbindTextures() override;

	void SetShader(IShader* shader) override { m_Shader = shader; }
	IShader* GetShader() override { return m_Shader; }

	glm::vec3 GetColor() override { return m_Color; }
	void SetColor(const glm::vec3& color) override { m_Color = color; }

	MaterialType GetType() { return m_Type; }
	void SetType(const MaterialType& type);

	void SetName(const std::string& name) override { m_Name = name; }
	std::string GetName() override { return m_Name; }

	void SetAlbedo(const glm::vec3& albedo) { m_Albedo = albedo; }
	void SetMetallic(const float& metallic) { m_Metallic = metallic; }
	void SetRoughness(const float& roughness) { m_Roughness = roughness; }
	void SetAo(const float& ambientOcclusion) { m_Ao = ambientOcclusion; }

	void SetAmbient(const glm::vec3& ambient) { m_Ambient = ambient; }
	glm::vec3* GetAmbient() { return &m_Ambient; }

	void SetDiffuse(const glm::vec3& diffuse) { m_Diffuse = diffuse; }
	glm::vec3* GetDiffuse() { return &m_Diffuse; }

	void SetSpecular(const glm::vec3 &specular) { m_Specular = specular; }
	glm::vec3* GetSpecular() { return &m_Specular; }

	void SetShininess(const float& shininess) { m_Shininess = shininess; }
	float *GetShininess() { return &m_Shininess; }

	void SetHasAmbient(const bool& hasAmbient) { m_HasAmbient = hasAmbient; }
	bool GetHasAmbient() { return m_HasAmbient; }

	void SetHasDiffuse(const bool& hasDiffuse) { m_HasDiffuse = hasDiffuse; }
	bool GetHasDiffuse() { return m_HasDiffuse; }

	void SetHasSpecular(const bool& hasSpecular) { m_HasSpecular = hasSpecular; }
	bool GetHasSpecular() { return m_HasSpecular; }

	void SetReflections(ReflectionType r);

private:

	std::string m_Name;
	MaterialType m_Type;
	IShader* m_Shader;


	// Phong Shading properties
	glm::vec3 m_Color;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Shininess;

	int m_HasTexture;
	int m_HasAmbient;
	int m_HasDiffuse;
	int m_HasSpecular;

	// Physically Based Rendering properties
	glm::vec3 m_Albedo;
	float m_Metallic;
	float m_Roughness;
	float m_Ao;
	
	ITexture* m_TextureAlbedo = nullptr;
	ITexture* m_TextureMetallic = nullptr;
	ITexture* m_TextureRoughness = nullptr;
	ITexture* m_TextureNormal = nullptr;
	ITexture* m_TextureAmbienOcclusion = nullptr;
	ITexture* m_TextureIrradiance = nullptr;
	ITexture* m_TexturePrefilter = nullptr;
	ITexture* m_TextureBrdfLookUp = nullptr;
	ITexture* m_TextureShadowDepth = nullptr;

	ReflectionProbe* m_Probe = nullptr;

};


class MaterialLoader
{
public:
	MaterialLoader() = default;

	void LoadMaterialFolder(const std::string& path);

	std::vector<std::filesystem::path> m_Entrys;
};