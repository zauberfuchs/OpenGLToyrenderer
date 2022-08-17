#pragma once

#include <GL/glew.h>
#include <stb_image.h>

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
	Texture2DMultiSample = GL_TEXTURE_2D_MULTISAMPLE,
	TextureCubeMap = GL_TEXTURE_CUBE_MAP
};

enum class TextureWrap
{
	Repeat = GL_REPEAT,
	ClampToEdge = GL_CLAMP_TO_EDGE
};

enum class TextureFilter
{
	Linear = GL_LINEAR,
	Nearest = GL_NEAREST,
	MipMapLinear = GL_LINEAR_MIPMAP_LINEAR
};


class Texture
{

public:
	Texture() = default;
	Texture(const std::string& path, TextureType type);
	~Texture();
	
	TextureType GetTextureType() { return m_Type; }
	void SetTextureType(const TextureType& type) { m_Type = type; }

	std::string GetName() { return m_Name; }
	void SetName(const std::string name) { m_Name = name; }

	void Load();
	void Create(const std::string& path, const TextureTarget& tt, const TextureWrap& tw, const TextureFilter& tf, bool isFlipped = true);
	
	int GetWidth() const { return m_Width; }
	int GetHeigth() const { return m_Height; }

	int GetTextureID() const { return m_ID; }
	void SetTextureID(const int& id) { m_ID = id; }

	TextureTarget GetTextureTarget() const { return m_Target; }
	void SetTextureTarget(TextureTarget tt) { m_Target = tt; }

	std::string GetUniformLocation() { return m_UniformLocation; }
	void SetUniformLocation(const std::string& uniformLocation) { m_UniformLocation = uniformLocation; }

	void Bind(const unsigned int& slot) const;
	void Unbind() const;

	//Todo make  them private
	std::string m_FilePath;
	TextureType m_Type;
	TextureTarget m_Target;
	std::string m_UniformLocation;

private:
	GLuint m_ID;
	int m_Width, m_Height, m_Components;
	std::string m_Name;

};
