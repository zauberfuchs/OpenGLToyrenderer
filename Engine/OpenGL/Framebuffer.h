#pragma once

#include <GL/glew.h>
#include "Texture.h"

enum class FramebufferAttachment
{
	Depth = GL_DEPTH_ATTACHMENT,
	DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
};

enum class FramebufferTextureFormat
{
	Depth24 = GL_DEPTH_COMPONENT24,
	Depth32 = GL_DEPTH_COMPONENT32,
	Depth24Stencil8 = GL_DEPTH24_STENCIL8,
	Depth32Stencil8 = GL_DEPTH32F_STENCIL8,
};

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void Bind() const;
	void Unbind() const;
	void CreateColorTexture2D(const TextureTarget& tt, const TextureWrap& tw, const TextureFilter tf);
	
	void AttachColorTexture3D(const uint16_t& face, const Texture& tex, const uint16_t& mipMapLevel = 0);
	void AttachColorTexture2D(const Texture& tex);
	void CreateDepthView();
	GLuint GetColorTextureId();
	GLuint GetId();
	void AttachRenderBuffer(const GLuint& rbo_ID, const FramebufferAttachment& attachment);
	void SetFramebufferTextureSize(const int& width, const int& height);
	void SetSampleSize(unsigned int samples);

private:
	GLuint m_ID;
	GLuint m_ColorTextureID;
	GLuint m_DepthTextureID;

	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_SampleSize = 1;
};