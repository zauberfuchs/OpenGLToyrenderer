#pragma once

#include "../../Engine/Scene/ReflectionProbe.h"

#include "../../Engine/Scene/World.h"

enum class MaterialType
{
	PhysicallyBased,
	Phong,
	TexturedPhysicallyBased
};

enum class ReflectionType
{
	None = 0, Ambient, Diffuse, Specular, Phong
};


class Material
{
public:
	Material();
	Material(const std::string& name);

	~Material();
	

	void SetTexture(Texture* texture);
	Texture* GetTexture(const TextureType& channelMap);
	void SetPBRTexture(const std::string& path);

	void SetReflectionProbe(ReflectionProbe* probe);
	void UpdateReflectionProbe();

	void SetupTextures();
	void SetupUniforms();
	void UnbindTextures();

	void SetShader(Shader* shader) { m_Shader = shader; }
	Shader* GetShader() { return m_Shader; }

	glm::vec3 GetColor() { return m_Color; }
	void SetColor(const glm::vec3& color) { m_Color = color; }

	MaterialType GetType() { return m_Type; }
	void SetType(const MaterialType& type);

	void SetName(const std::string& name) { m_Name = name; }
	std::string GetName() { return m_Name; }

	void SetAlbedo(const glm::vec3& albedo) { m_Albedo = albedo; }
	glm::vec3 GetAlbedo() { return m_Albedo; }

	void SetMetallic(const float& metallic) { m_Metallic = metallic; }
	float GetMetallic() { return m_Metallic; }

	void SetRoughness(const float& roughness) { m_Roughness = roughness; }
	float GetRoughness() { return m_Roughness; }

	void SetAo(const float& ambientOcclusion) { m_Ao = ambientOcclusion; }
	float GetAo() { return m_Ao; }


	void SetAmbient(const glm::vec3& ambient) { m_Ambient = ambient; }
	glm::vec3 GetAmbient() { return m_Ambient; }

	void SetDiffuse(const glm::vec3& diffuse) { m_Diffuse = diffuse; }
	glm::vec3 GetDiffuse() { return m_Diffuse; }

	void SetSpecular(const glm::vec3 &specular) { m_Specular = specular; }
	glm::vec3 GetSpecular() { return m_Specular; }

	void SetShininess(const float& shininess) { m_Shininess = shininess; }
	float GetShininess() { return m_Shininess; }

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
	Shader* m_Shader;


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


	// Physically Based Textured Rendering propoerties
	Texture* m_TextureShadowDepth = nullptr;

	ReflectionProbe* m_Probe = nullptr;

	std::unordered_map<TextureType, Texture*> m_Textures;

};


class MaterialLoader
{
public:
	static void LoadMaterialFolder(const std::string& path);
	std::vector<std::filesystem::path> m_Entrys;
};