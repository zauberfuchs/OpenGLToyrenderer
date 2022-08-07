#pragma once

#include <GL/glew.h>
#include <stb_image.h>
#include "../../Engine/Interfaces/ITexture.h"

class Texture : public ITexture
{

public:
	Texture();
	Texture(const std::string& path, TextureType type);
	~Texture() override;

	static std::shared_ptr<Texture> Create(const std::string path, TextureType type);

	void Render(IShader* shader) override;
	void RenderPost() override;
	void RenderPre() override;

	void SetTextureType(TextureType type) override;
	TextureType GetTextureType() override;

	void SetName(const std::string name) override;
	std::string GetName() override;

	void Load() override;
	void LoadModelTexture(const std::string& path, TextureType type) override;

	inline int GetWidth() const override { return m_Width; }
	inline int GetHeigth() const override { return m_Height; }

	int GetTextureID() { return m_ID; }
	void SetTextureID(const int& id) { m_ID = id; }

protected:

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

public:
	std::string m_FilePath;
	TextureType m_Type;
private:
	GLuint m_ID;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_Components;
	std::string m_Name;

};
