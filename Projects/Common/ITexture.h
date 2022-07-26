#pragma once

#include "IShader.h"

#include <string>
#include <iostream>

enum class ETextureChannels {
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

	virtual void SetTextureType(ETextureChannels type) = 0;
	virtual ETextureChannels GetTextureType() = 0;

	virtual void SetName(const std::string name) = 0;
	virtual std::string GetName() = 0;

	virtual void Load() = 0;
	virtual void LoadModelTexture(const std::string& path, ETextureChannels type) = 0;

	virtual void Bind(unsigned int slot = 0) const = 0;
	virtual void Unbind() const = 0;

	virtual inline int GetWidth() const = 0;
	virtual inline int GetHeigth() const = 0;
};
