#define STB_IMAGE_IMPLEMENTATION
#include "../../Engine/Utils/pch.h"
#include "Texture.h"


Texture::Texture(const std::string& path, TextureType type)
	: m_FilePath(path), m_Type(type)
{
	Load();
}

Texture::Texture()
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::Load()
{
	stbi_set_flip_vertically_on_load(1); // flipt die Texture damit es von unten anfängt
	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_Components, 0);

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum format, internal_format;
	if (m_Components == 1)
	{
		format = GL_RED;
		internal_format = GL_R8;
	}
	else if (m_Components == 3)
	{
		format = GL_RGB;
		internal_format = GL_RGB;
	}
	else if (m_Components == 4)
	{
		format = GL_RGBA;
		internal_format = GL_RGBA;
	}
	else
	{
		std::cout << "Texture loading not implemented for this number of compenents.\n";
		exit(1);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

void Texture::LoadModelTexture(const std::string& path, TextureType type)
{
	m_FilePath = path;
	m_Type = type;

	glGenTextures(1, &m_ID);

	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_Components, 0);
	if (m_LocalBuffer)
	{
		GLenum format;
		if (m_Components == 1)
			format = GL_RED;
		else if (m_Components == 3)
			format = GL_RGB;
		else if (m_Components == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(m_LocalBuffer);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << m_FilePath << std::endl;
		stbi_image_free(m_LocalBuffer);
	}
}



void Texture::Bind(unsigned int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot); // der Texture Slot hier 0 = erster
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::RenderPre()
{

	switch (m_Type) {
	case TextureType::AlbedoMap:
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		break;
	case TextureType::NormalMap:
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		break;
	case TextureType::MetallicMap:
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		break;
	case TextureType::RoughnessMap:
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		break;
	case TextureType::AmbientOcclusionMap:
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		break;
	case TextureType::IrradianceMap:
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
		break;
	case TextureType::PrefilterMap:
		glActiveTexture(GL_TEXTURE0 + 6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
		break;
	case TextureType::BrdfLookUpTexture:
		glActiveTexture(GL_TEXTURE0 + 7);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		break;
	case TextureType::DepthMap:
		glActiveTexture(GL_TEXTURE0 + 8);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
		break;
	default:
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		break;
	}
}

void Texture::SetName(const std::string name)
{
	m_Name = name;
}
std::string Texture::GetName()
{
	return m_Name;
}

void Texture::Render(IShader* shader)
{
}

void Texture::RenderPost()
{
	glActiveTexture(GL_TEXTURE0);
}

TextureType Texture::GetTextureType()
{
	return m_Type;
}

void Texture::SetTextureType(TextureType type)
{
	m_Type = type;
}