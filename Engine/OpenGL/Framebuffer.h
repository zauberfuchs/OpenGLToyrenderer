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

enum class FramebufferColorBuffer
{
	None = GL_NONE
};

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void Bind() const;
	void Unbind() const;

	void AttachDepthTexture(const Texture& tex);
	void AttachColorTexture2D(const Texture& tex);
	void AttachColorTexture3D(const uint16_t& face, const Texture& tex, const uint16_t& mipMapLevel = 0);

	//void CreateDepthView();
	GLuint GetId() { return m_ID; }

	void SetDrawBuffer(const FramebufferColorBuffer& fc);
	void SetReadBuffer(const FramebufferColorBuffer& fc);
	void AttachRenderBuffer(const GLuint& rbo_ID, const FramebufferAttachment& attachment);
	void SetSampleSize(const unsigned int& samples) { m_SampleSize = samples; }
	unsigned int GetSampleSize() { return m_SampleSize; }

private:
	GLuint m_ID;
	
	unsigned int m_SampleSize = 1;
};