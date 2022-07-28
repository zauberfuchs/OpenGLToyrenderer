#pragma once

#include "../../Engine/Interfaces/IMaterial.h"
#include "../../Engine/Scene/ReflectionProbe.h"

#include "../../Engine/Scene/World.h"



class MaterialPBR : public IMaterial
{
public:
	MaterialPBR();
	MaterialPBR(const std::string& name);

	~MaterialPBR() override;
	

	void SetTexture(ITexture* texture) override;
	void SetPBRTexture(const std::string& path);
	ITexture* GetTexture(ETextureChannels channelMap) override;

	void SetShader(IShader* shader) override;
	IShader* GetShader() override;

	void SetName(const std::string name) override;
	std::string GetName() override;

	void SetReflectionProbe(ReflectionProbe* probe);

	void RenderPre() override;
	void Render() override;
	void RenderPost() override;

	inline glm::vec3 GetColor() override { return m_Color; }
	inline void SetColor(const glm::vec3& color) override { m_Color = color; }


	inline void SetAlbedo(const glm::vec3& albedo) { m_Albedo = albedo; }
	inline void SetMetallic(const float& metallic) { m_Metallic = metallic; }
	inline void SetRoughness(const float& roughness) { m_Roughness = roughness; }
	inline void SetAo(const float& ambientOcclusion) { m_Ao = ambientOcclusion; }


private:
	glm::vec3 m_Color;
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

	IShader* m_Shader;

	std::string m_Name;
};


class MaterialLoader
{
public:
	MaterialLoader() = default;

	void LoadMaterialFolder(const std::string& path);

	std::vector<std::filesystem::path> m_Entrys;
};