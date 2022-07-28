#pragma once

#include <GL/glew.h>


enum class FramebufferAttachment
{
	Depth = GL_DEPTH_ATTACHMENT,
	DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
};

class Framebuffer
{
public:
	Framebuffer();
	Framebuffer(const int& width, const int& height);
	~Framebuffer();

	void Bind() const;
	void Unbind() const;
	void CreateColorTexture(bool isMultisampled);
	void CreateDepthView();
	GLuint GetColorTextureId();
	GLuint GetId();
	void AttachRenderBuffer(const GLuint& rbo_ID, const FramebufferAttachment& attachment);
	void SetSampleSize(unsigned int samples);

private:
	GLuint m_ID;
	GLuint m_ColorTextureID;
	GLuint m_DepthTextureID;

	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_SampleSize = 1;
};