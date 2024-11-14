#pragma once

#include <GL/glew.h>
#include <stb_image.h>


enum class TextureType : uint32
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

enum class TextureInternalFormat : uint32
{
	R8 = GL_R8,
	Rgb = GL_RGB,
	Rgb8 = GL_RGB8,
	Rgb16 = GL_RGB16,
	Rg16F = GL_RG16F,
	Rgb16F = GL_RGB16F,
	Rgb32 = GL_RGB32I,
	Rgb32F = GL_RGB32F,
	Rgba8 = GL_RGBA8,
	D16 = GL_DEPTH_COMPONENT16
};

enum class TextureTarget : uint32
{
	Texture2D = GL_TEXTURE_2D,
	Texture2DMultiSample = GL_TEXTURE_2D_MULTISAMPLE,
	TextureCubeMap = GL_TEXTURE_CUBE_MAP,
	TextureCubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY
};

enum class TextureWrap
{
	Repeat = GL_REPEAT,
	ClampToEdge = GL_CLAMP_TO_EDGE,
	ClampToBorder = GL_CLAMP_TO_BORDER
};

enum class TextureFilter
{
	Linear = GL_LINEAR,
	Nearest = GL_NEAREST,
	MipMapLinear = GL_LINEAR_MIPMAP_LINEAR
};

struct TextureDesc
{
	TextureType 				Type;
	TextureTarget 				Target;
	TextureInternalFormat 		Format;
	uint32						Width;
	uint32						Height;
	uint32 						MipLevels;
	uint32						DepthOrArraySize;
	uint32						SampleCount;
	const char*					DebugName;
};

class Texture
{

public:
	Texture() = default;
	Texture(const TextureTarget& tt);
	Texture(const TextureDesc& descTexture);
	Texture(const std::string& path, const TextureTarget& tt);
	~Texture();
	
	void Load();
	void Load(const std::string& path, const TextureWrap& tw, const TextureFilter& tf, bool isFlipped = true);
	void LoadCubemap(const std::string& path);
	void CreateTextureStorage();
	void CreateTexture2DStorage(const TextureInternalFormat& tif, const bool& hasMipMap = false, const uint16_t& samples = 0);
	void CreateTextureCubeMapStorage(const TextureInternalFormat& tif, const bool& hasMipMap = false);
	void GenerateMipMap() const;

	void SetWrapMode(const TextureWrap& s, const TextureWrap& t) const;
	void SetWrapMode(const TextureWrap& s, const TextureWrap& t, const TextureWrap& r) const;
	void SetFilter(const TextureFilter& min, const TextureFilter& mag) const;
	void SetBorderColor(const float* borderColor) const;
	void SetTexture2DByteData(const char* byteArray) const;

	void SaveTextureToCache(void* data);
	unsigned char* LoadTextureFromCache();
	

	TextureType GetTextureType() { return m_Type; }
	void SetTextureType(const TextureType& type) { m_Type = type; }

	std::string GetName() { return m_Name; }
	void SetName(const std::string name) { m_Name = name; }

	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

	void SetTexture2DSize(const int& width, const int& height) { m_Width = width; m_Height = height; }

	int GetTextureID() const { return m_ID; }
	void SetTextureID(const int& id) { m_ID = id; }

	TextureTarget GetTextureTarget() const { return m_Target; }
	void SetTextureTarget(TextureTarget tt) { m_Target = tt; }

	std::string GetUniformLocation() { return m_UniformLocation; }
	void SetUniformLocation(const std::string& uniformLocation) { m_UniformLocation = uniformLocation; }

	void Bind(const unsigned int& slot = 0) const;
	void Unbind() const;

	std::string GetFilePath() { return m_FilePath; }
	
private:
	GLuint m_ID;
	TextureType m_Type;
	TextureTarget m_Target;
	TextureDesc m_Desc;

	std::string m_Name;
	std::string m_FilePath;
	std::string m_UniformLocation;

	uint16_t m_MipMapLevel;
	int m_Width, m_Height, m_Components;
};
