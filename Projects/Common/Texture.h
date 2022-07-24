#pragma once

#include <GL/glew.h>
#include <stb_image.h>
#include <string>
#include <iostream>
#include "ITexture.h"
#include <array>

class Texture : public ITexture
{

public:
	Texture();
	Texture(const std::string& path, ETextureChannels type);
	~Texture() override;

	static std::shared_ptr<Texture> Create(const std::string path, ETextureChannels type);

	void Render(IShader* shader) override;
	void RenderPost() override;
	void RenderPre() override;

	void SetTextureType(ETextureChannels type) override;
	ETextureChannels GetTextureType() override;

	void SetName(const std::string name) override;
	std::string GetName() override;

	void Load() override;
	void LoadModelTexture(const std::string& path, ETextureChannels type) override;

	inline int GetWidth() const override { return m_Width; }
	inline int GetHeigth() const override { return m_Height; }

protected:

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

public:
	GLuint m_ID;
	std::string m_FilePath;
	ETextureChannels m_Type;
private:
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_Components;
	std::string m_Name;

};
