#include "Engine/Utils/pch.h"
#include "Framebuffer.h"


Framebuffer::Framebuffer()
{
	glCreateFramebuffers(1, &m_ID);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_ID);
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void Framebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Framebuffer::GetColorTextureId() 
{
	return m_ColorTextureID;
}

GLuint Framebuffer::GetId()
{
	return m_ID;
}

void Framebuffer::AttachDepthTexture(const Texture& tex)
{
	glNamedFramebufferTexture(m_ID, GL_DEPTH_ATTACHMENT, tex.GetTextureID(), 0);
}

void Framebuffer::AttachColorTexture2D(const Texture& tex)
{
	glNamedFramebufferTexture2DEXT(m_ID, GL_COLOR_ATTACHMENT0, static_cast<GLint>(tex.GetTextureTarget()), tex.GetTextureID(), 0);
}

void Framebuffer::AttachColorTexture3D(const uint16_t& face, const Texture& tex, const uint16_t& mipMapLevel)
{
	glNamedFramebufferTexture2DEXT(m_ID, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, tex.GetTextureID(), mipMapLevel);
}

void Framebuffer::SetDrawBuffer(const FramebufferColorBuffer& fc)
{
	glNamedFramebufferDrawBuffer(m_ID, static_cast<GLint>(fc));
}
void Framebuffer::SetReadBuffer(const FramebufferColorBuffer& fc)
{
	glNamedFramebufferReadBuffer(m_ID, static_cast<GLint>(fc));
}

void Framebuffer::AttachRenderBuffer(const GLuint& rbo_ID, const FramebufferAttachment& attachment)
{
	glNamedFramebufferRenderbuffer(m_ID, static_cast<GLenum>(attachment), GL_RENDERBUFFER, rbo_ID);
}

void Framebuffer::SetSampleSize(unsigned int samples)
{
	m_SampleSize = samples;
}

void Framebuffer::SetFramebufferTextureSize(const int& width, const int& height)
{
	m_Width = width;
	m_Height = height;
}

//void Framebuffer::CreateDepthView()
//{
//	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
//	/*Texture tex(TextureTarget::Texture2D);
//	tex.SetTexture2DSize(m_Width, m_Height);
//	tex.SetWrapMode();
//	tex.SetFilter(TextureFilter::Nearest);
//	tex.CreateTexture2DStorage();*/
//
//	//glCreateTextures();
//
//	glGenTextures(1, &m_DepthTextureID);
//	glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
//	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
//	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0);
//	//glNamedFramebufferTexture2DEXT(m_ID, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
//	glNamedFramebufferTexture2DEXT(m_ID, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0);
//	glNamedFramebufferDrawBuffer(m_ID, GL_NONE);
//	glNamedFramebufferReadBuffer(m_ID, GL_NONE);
//
//	/*Bind();
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0);
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
//}


