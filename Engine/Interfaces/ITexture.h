#pragma once

#include <GL/glew.h>

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

enum class TextureTarget
{
	Texture2D = GL_TEXTURE_2D,
	TextureCubeMap = GL_TEXTURE_CUBE_MAP
};



class ITexture
{

public:
	virtual ~ITexture() = default;

	virtual void SetTextureType(const TextureType& type) = 0;
	virtual TextureType GetTextureType() = 0;

	virtual void SetName(const std::string name) = 0;
	virtual std::string GetName() = 0;
	virtual int GetTextureID() = 0;
	virtual void SetTextureID(const int& id) = 0;
	virtual void Load() = 0;

	virtual TextureTarget GetTextureTarget() = 0;
	virtual void SetTextureTarget(TextureTarget tt) = 0;

	virtual std::string GetUniformLocation() = 0;
	virtual void SetUniformLocation(const std::string& uniformLocation) = 0;

	virtual void Bind(const unsigned int& slot, TextureTarget tt) const = 0;
	virtual void Unbind() const = 0;

	virtual inline int GetWidth() const = 0;
	virtual inline int GetHeigth() const = 0;
};
