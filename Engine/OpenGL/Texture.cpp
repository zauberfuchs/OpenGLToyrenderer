#define STB_IMAGE_IMPLEMENTATION
#include "Engine/Utils/pch.h"
#include "Texture.h"


Texture::Texture(const std::string& path, TextureType type)
	: m_FilePath(path), m_Type(type)
{
	Load();
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::Load()
{
	stbi_set_flip_vertically_on_load(1); // flipt die Texture damit es von unten anfängt
	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_Components, 0);

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
	
	glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glGenerateTextureMipmap(m_ID);

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

void Texture::Bind(const unsigned int& slot, TextureTarget tt) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(static_cast<GLenum>(tt), m_ID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}