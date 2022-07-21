#pragma once

#include <GL/glew.h>
#include <stb_image.h>

#include "IMaterial.h"
#include "ITexture.h"
#include <string>
#include <iostream>



class Material : public IMaterial
{
public:
	Material();
	Material(const std::string& name);

	~Material() override;
	

	void SetTexture(ITexture* texture) override;
	ITexture* GetTexture(ETextureChannels channelMap = ETextureChannels::AlbedoMap) override;

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
	ITexture* m_TextureDiff = nullptr;
	ITexture* m_TextureSpec = nullptr;
	ITexture* m_TextureAO = nullptr;
	ITexture* m_TextureBump = nullptr;
	ITexture* m_TextureNorm = nullptr;
	ITexture* m_TextureHeightMap = nullptr;

	IShader* m_Shader;

	std::string m_Name;
};
