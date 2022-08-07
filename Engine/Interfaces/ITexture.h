#pragma once

#include "IShader.h"


enum class TextureType
{
	SpecularMap,
	AmbientOcclusionMap,
	NormalMap,
	HeightMap,
	EnvironmentMap,
	IrradianceMap,
	ReflectionMap,
	MetallicMap,
	RoughnessMap,
	AlbedoMap,
	EmissiveMap,
	BrdfLookUpTexture,
	DepthMap,
	PrefilterMap
};



class ITexture
{

public:
	virtual ~ITexture() = default;

	virtual void RenderPre() = 0;
	virtual void Render(IShader* shader) = 0;
	virtual void RenderPost() = 0;

	virtual void SetTextureType(TextureType type) = 0;
	virtual TextureType GetTextureType() = 0;

	virtual void SetName(const std::string name) = 0;
	virtual std::string GetName() = 0;
	virtual int GetTextureID() = 0;
	virtual void SetTextureID(const int& id) = 0;
	virtual void Load() = 0;
	virtual void LoadModelTexture(const std::string& path, TextureType type) = 0;

	virtual void Bind(unsigned int slot = 0) const = 0;
	virtual void Unbind() const = 0;

	virtual inline int GetWidth() const = 0;
	virtual inline int GetHeigth() const = 0;
};
