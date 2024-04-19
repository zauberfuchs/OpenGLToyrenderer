#define STB_IMAGE_IMPLEMENTATION
#include "Engine/Utils/pch.h"
#include "Texture.h"


Texture::Texture(const std::string& path, const TextureTarget& tt)
	: m_FilePath(path), m_Target(tt), m_ID(0), m_MipMapLevel(0)
{
	m_Name = std::filesystem::path(m_FilePath).stem().string();
	Load();
}

Texture::Texture(const TextureTarget& tt)
	: m_Target(tt), m_ID(0), m_MipMapLevel(0)
{
	//glCreateTextures(static_cast<GLenum>(m_Target), 1, &m_ID);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::SaveTextureToCache(void* data) 
{
	if (data) {
		// Open the output file stream
		std::string materialName = std::filesystem::path(m_FilePath).parent_path().filename().string();


		std::ofstream outputFile("../Data/Textures/Loaded/" + materialName + m_Name + ".fTex", std::ios::out | std::ios::binary);
		
		outputFile.write((char*)&m_Width, sizeof(int));
		outputFile.write((char*)&m_Height, sizeof(int));
		outputFile.write((char*)&m_Components, sizeof(int));

		int dataSize = m_Width * m_Height * m_Components;
		// Write the image data to the file
		outputFile.write((char*)data, dataSize);
		
		// Close the output file stream
		outputFile.close();
	}
	else {
		std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
	}
}

unsigned char* Texture::LoadTextureFromCache()
{
	// Open the input file stream
	std::string materialName = std::filesystem::path(m_FilePath).parent_path().filename().string();
	std::ifstream inputFile("../Data/Textures/Loaded/" + materialName + m_Name + ".fTex", std::ios::in | std::ios::binary);

	if (inputFile) {

		// Read the image metadata from the file
		inputFile.read((char*)&m_Width, sizeof(int));
		inputFile.read((char*)&m_Height, sizeof(int));
		inputFile.read((char*)&m_Components, sizeof(int));

		/// Move the read pointer to the start of the image data
		inputFile.seekg(sizeof(int) * 3, std::ios::beg);

		// Calculate the size of the image data
		int dataSize = m_Width * m_Height * m_Components;

		// Allocate memory for the image data
		unsigned char* imageData = new unsigned char[dataSize];

		// Read the image data from the file
		inputFile.read(reinterpret_cast<char*>(imageData), dataSize);

		// Close the input file stream
		inputFile.close();


		// Return the image data
		return imageData;
	}
	else 
	{
		std::cerr << "Error opening file: " << m_Name << std::endl;
		return nullptr;
	}
}

void Texture::Load()
{
	void* data;
	GLenum dataFormat;

	std::string materialName = std::filesystem::path(m_FilePath).parent_path().filename().string();
	if (std::filesystem::exists("../Data/Textures/Loaded/" + materialName + m_Name + ".fTex")) 
	{
		data = LoadTextureFromCache();
		dataFormat = GL_UNSIGNED_BYTE;
	}
	else 
	{
		if (m_FilePath.find_first_of(".hdr"))
		{
			data = stbi_loadf(m_FilePath.c_str(), &m_Width, &m_Height, &m_Components, 0);
			dataFormat = GL_FLOAT;
		}
		else
		{
			data = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_Components, 0);
			dataFormat = GL_UNSIGNED_BYTE;
		}
		SaveTextureToCache(data);
	}

	stbi_set_flip_vertically_on_load(1);

	glCreateTextures(static_cast<GLenum>(m_Target), 1, &m_ID);

	const auto mipmapLevels = static_cast<GLsizei>(glm::floor(glm::log2(glm::max(m_Width, m_Height))));

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
	
	glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, format, dataFormat, data);
	glGenerateTextureMipmap(m_ID);

	if (data)
		stbi_image_free(data);
}

void Texture::Load(const std::string& path, const TextureWrap& tw, const TextureFilter& tf, bool isFlipped)
{
	m_Name = std::filesystem::path(path).stem().string();
	stbi_set_flip_vertically_on_load(isFlipped); // flipt die Texture damit es von unten anf�ngt
	float* data = stbi_loadf(path.c_str(), &m_Width, &m_Height, &m_Components, 0);

	glCreateTextures(static_cast<GLenum>(m_Target), 1, &m_ID);
	
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

void Texture::LoadCubemap(const std::string& path)
{
	stbi_set_flip_vertically_on_load(0);

	unsigned char* data;

	std::vector<std::string> faces
	{
		"/right.png",
		"/left.png",
		"/top.png",
		"/bottom.png",
		"/front.png",
		"/back.png"
	};

	if(m_ID)
	{
		glDeleteTextures(1, &m_ID);
	}
	glCreateTextures(static_cast<GLenum>(m_Target), 1, &m_ID);

	glTextureStorage2D(m_ID, 1, GL_RGBA8, 1024, 1024);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		data = stbi_load((path + faces[i]).c_str(), &m_Width, &m_Height, &m_Components, 0);
		if (data)
		{
			glTextureSubImage3D(m_ID, 0, 0, 0, i, m_Width, m_Height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << path + faces[i] << std::endl;
		}
	}
	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::CreateTexture2DStorage(const TextureInternalFormat& tif, const bool& hasMipMap, const uint16_t& samples) 
{
	if(m_ID)
	{
		glDeleteTextures(1, &m_ID);
	}
	glCreateTextures(static_cast<GLenum>(m_Target), 1, &m_ID);

	if (hasMipMap)
	{
		const auto mipmapLevels = static_cast<unsigned int>(glm::floor(glm::log2(glm::max(m_Width, m_Height))));
		glTextureStorage2D(m_ID, mipmapLevels, static_cast<GLenum>(tif), m_Width, m_Height);
	}
	else
	{
		if (m_Target == TextureTarget::Texture2DMultiSample)
		{
			glTextureStorage2DMultisample(m_ID, samples, static_cast<GLenum>(tif), m_Width, m_Height, GL_TRUE);
		}
		else
		{
			glTextureStorage2D(m_ID, 1, static_cast<GLenum>(tif), m_Width, m_Height);
		}
	}
}

void Texture::CreateTextureCubeMapStorage(const TextureInternalFormat& tif, const bool& hasMipMap)
{
	if (m_ID)
	{
		glDeleteTextures(1, &m_ID);
	}
	glCreateTextures(static_cast<GLenum>(m_Target), 1, &m_ID);

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

void Texture::GenerateMipMap() const
{
	glGenerateTextureMipmap(m_ID);
}

void Texture::SetWrapMode(const TextureWrap& s, const TextureWrap& t) const
{
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
}

void Texture::SetWrapMode(const TextureWrap& s, const TextureWrap& t, const TextureWrap& r) const
{
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, static_cast<GLint>(r));
}

void Texture::SetFilter(const TextureFilter& min, const TextureFilter& mag) const
{
	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
}

void Texture::SetBorderColor(const float* borderColor) const
{
	glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, borderColor);
}


void Texture::Bind(const unsigned int& slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(static_cast<GLenum>(m_Target), m_ID);
}

void Texture::Unbind() const
{
	glBindTexture(static_cast<GLenum>(m_Target), 0);
}