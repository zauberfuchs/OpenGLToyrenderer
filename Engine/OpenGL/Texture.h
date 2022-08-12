#pragma once

#include <GL/glew.h>
#include <stb_image.h>
#include "../../Engine/Interfaces/ITexture.h"

class Texture : public ITexture
{

public:
	Texture() = default;
	Texture(const std::string& path, TextureType type);
	~Texture() override;
	
	TextureType GetTextureType() override { return m_Type; }
	void SetTextureType(const TextureType& type) override { m_Type = type; }

	std::string GetName() override { return m_Name; }
	void SetName(const std::string name) override { m_Name = name; }

	void Load() override;
	
	int GetWidth() const override { return m_Width; }
	int GetHeigth() const override { return m_Height; }

	int GetTextureID() override { return m_ID; }
	void SetTextureID(const int& id) override { m_ID = id; }

	TextureTarget GetTextureTarget() override { return m_Target; }
	void SetTextureTarget(TextureTarget tt) override { m_Target = tt; }

	std::string GetUniformLocation() override { return m_UniformLocation; }
	void SetUniformLocation(const std::string& uniformLocation) override { m_UniformLocation = uniformLocation; }
protected:

	void Bind(const unsigned int& slot, TextureTarget tt) const;
	void Unbind() const;

public:
	std::string m_FilePath;
	TextureType m_Type;
	TextureTarget m_Target;
	std::string m_UniformLocation;
private:
	GLuint m_ID;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_Components;
	std::string m_Name;

};
