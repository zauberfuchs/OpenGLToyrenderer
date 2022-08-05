#pragma once

#include "ITexture.h"

enum class ReflectionType
{
	None = 0, Ambient, Diffuse, Specular, Phong
};

class IMaterial
{
public:
	virtual ~IMaterial() = default;

	virtual void SetName(const std::string name) = 0;
	virtual std::string GetName() = 0;
	
	virtual void SetTexture(ITexture* texture) = 0;
	virtual ITexture* GetTexture(TextureType channelMap = TextureType::AlbedoMap) = 0;

	virtual void SetShader(IShader* shader) = 0;
	virtual IShader* GetShader() = 0;

	virtual glm::vec3 GetColor() = 0;
	virtual void SetColor(const glm::vec3& color) = 0;

	virtual void RenderPre() = 0;
	virtual void Render() = 0;
	virtual void RenderPost() = 0;

};
