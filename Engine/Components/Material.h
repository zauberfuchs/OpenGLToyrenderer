#pragma once

#include "../../Engine/Interfaces/IMaterial.h"




class Material : public IMaterial
{
public:
	Material();
	Material(const std::string& name);

	~Material() override;
	

	void SetTexture(ITexture* texture) override;
	ITexture* GetTexture(TextureType channelMap = TextureType::AlbedoMap) override;

	void SetShader(IShader* shader) override;
	IShader* GetShader() override;

	glm::vec3 GetColor() override;
	void SetColor(const glm::vec3& color) override;

	void SetName(const std::string name) override;
	std::string GetName() override;

	void RenderPre() override;
	void Render() override;
	void RenderPost() override;

	void SetReflections(ReflectionType r);


	//ToDo: move to constructor
	glm::vec3 m_Color = glm::vec3(1.0f);
	glm::vec3 Ambient = glm::vec3(0.0f);
	glm::vec3 Diffuse = glm::vec3(0.0f);
	glm::vec3 Specular = glm::vec3(0.0f);
	float Shininess = 0.0f;

	int m_HasTexture = 0;
	int m_HasAmbient = 0;
	int m_HasDiffuse = 0;
	int m_HasSpecular = 0;

	int m_HasDiffuseMap = 0;
	int m_HasSpecularMap = 0;


protected:
	ITexture* m_TextureAlbedo = nullptr;

	IShader* m_Shader;

	std::string m_Name;
};
