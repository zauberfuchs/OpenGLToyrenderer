#define STB_IMAGE_IMPLEMENTATION
#include "Engine/Utils/pch.h"
#include "Texture.h"


Texture::Texture(const std::string& path)
	: m_FilePath(path)
{
	Load();
}

Texture::Texture(TextureTarget tt)
	: m_Target(tt)
{
	glCreateTextures(static_cast<GLenum>(tt), 1, &m_ID);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::Load()
{
	stbi_set_flip_vertically_on_load(1); // flipt die Texture damit es von unten anf�ngt
	unsigned char* data = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_Components, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

	const unsigned int mipmapLevels = static_cast<unsigned int>(glm::floor(glm::log2(glm::max(m_Width, m_Height))));

	GLenum format, internalFormat;
	if (m_Components == 1)
	{
		format = GL_RED;
		internalFormat = GL_R8;
	}
	else if (m_Components == 3)
	{
		format = GL_RGB;
		internalFormat = GL_RGB8;
	}
	else if (m_Components == 4)
	{
		format = GL_RGBA;
		internalFormat = GL_RGBA8;
	}
	else
	{
		std::cout << "Texture loading not implemented for this number of compenents.\n";
		exit(1);
	}

	glTextureStorage2D(m_ID, mipmapLevels, internalFormat, m_Width, m_Height);
	
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);
	glGenerateTextureMipmap(m_ID);

	if (data)
		stbi_image_free(data);
}

void Texture::Create(const std::string& path, const TextureTarget& tt, const TextureWrap& tw, const TextureFilter& tf, bool isFlipped)
{
	//load nicht create? weil er l�dt eine textur!
	m_Target = tt;
	stbi_set_flip_vertically_on_load(isFlipped); // flipt die Texture damit es von unten anf�ngt
	float* data = stbi_loadf(path.c_str(), &m_Width, &m_Height, &m_Components, 0);

	glCreateTextures(static_cast<GLenum>(tt), 1, &m_ID);
	
	GLenum format, internalFormat;
	if (m_Components == 1)
	{
		format = GL_RED;
		internalFormat = GL_R8;
	}
	else if (m_Components == 3)
	{
		format = GL_RGB;
		internalFormat = GL_RGB16F;
	}
	else if (m_Components == 4)
	{
		format = GL_RGBA;
		internalFormat = GL_RGBA8;
	}
	else
	{
		std::cout << "Texture loading not implemented for this number of compenents.\n";
		exit(1);
	}
	
	glTextureStorage2D(m_ID, 1, internalFormat, m_Width, m_Height);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, static_cast<GLint>(tw));
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, static_cast<GLint>(tw));
	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(tf));
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(tf));

	glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, format, GL_FLOAT, data);

	if (data)
		stbi_image_free(data);
}

void Texture::CreateTexture2DStorage(const TextureInternalFormat& tif, const bool& hasMipMap) const
{
	if (hasMipMap)
	{
		const auto mipmapLevels = static_cast<unsigned int>(glm::floor(glm::log2(glm::max(m_Width, m_Height))));
		glTextureStorage2D(m_ID, mipmapLevels, static_cast<GLenum>(tif), m_Width, m_Height);
	}else
	{
		glTextureStorage2D(m_ID, 1, static_cast<GLenum>(tif), m_Width, m_Height);
	}
}

void Texture::CreateTextureCubeMapStorage(const TextureInternalFormat& tif, const bool& hasMipMap) const
{
	if (hasMipMap)
	{
		const auto mipmapLevels = static_cast<unsigned int>(glm::floor(glm::log2(glm::max(m_Width, m_Height))));
		glTextureStorage2D(m_ID, mipmapLevels, static_cast<GLenum>(tif), m_Width, m_Height);
	}
	else
	{
		glTextureStorage2D(m_ID, 1, static_cast<GLenum>(tif), m_Width, m_Height);
	}
}

void Texture::GenerateMipMap()
{
	glGenerateTextureMipmap(m_ID);
}

void Texture::SetWrapMode(const TextureWrap& s, const TextureWrap& t)
{
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
}

void Texture::SetWrapMode(const TextureWrap& s, const TextureWrap& t, const TextureWrap& r)
{
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, static_cast<GLint>(r));
}

void Texture::SetFilter(const TextureFilter& min, const TextureFilter& mag)
{
	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
}

void Texture::Bind(const unsigned int& slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(static_cast<GLenum>(m_Target), m_ID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}