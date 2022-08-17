#include "Engine/Utils/pch.h"
#include "Framebuffer.h"


Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &m_ID);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_ID);
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

GLuint Framebuffer::GetColorTextureId() 
{
	return m_ColorTextureID;
}

GLuint Framebuffer::GetId()
{
	return m_ID;
}

void Framebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::CreateColorTexture(const TextureTarget& tt, const TextureWrap& tw, const TextureFilter tf)
{
	//Todo function zum übersetzten von GLenum => meinen internen Enums
	Bind();
	if (static_cast<GLint>(tt) == GL_TEXTURE_2D_MULTISAMPLE) {
		glDeleteTextures(1, &m_ColorTextureID);
		glGenTextures(1, &m_ColorTextureID);
		glBindTexture(static_cast<GLint>(tt), m_ColorTextureID);

		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_SampleSize, GL_RGB, m_Width, m_Height, GL_TRUE);
		
		glTexParameteri(static_cast<GLint>(tt), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(tf));
		glTexParameteri(static_cast<GLint>(tt), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(tf));

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorTextureID, 0);
		glBindTexture(static_cast<GLint>(tt), 0);

	}
	else if (static_cast<GLint>(tt) == GL_TEXTURE_CUBE_MAP) 
	{

		glGenTextures(1, &m_ColorTextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ColorTextureID);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glCreateTextures(static_cast<GLint>(tt), 1, &m_ColorTextureID);

		glTextureStorage2D(m_ColorTextureID, 1, GL_RG16F, m_Width, m_Height);

		glTextureParameteri(m_ColorTextureID, GL_TEXTURE_WRAP_S, static_cast<GLint>(tw));
		glTextureParameteri(m_ColorTextureID, GL_TEXTURE_WRAP_R, static_cast<GLint>(tw));
		glTextureParameteri(m_ColorTextureID, GL_TEXTURE_WRAP_T, static_cast<GLint>(tw));
		glTextureParameteri(m_ColorTextureID, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(tf));
		glTextureParameteri(m_ColorTextureID, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(tf));

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, static_cast<GLint>(tt), m_ColorTextureID, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Framebuffer::CreateDepthView()
{
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glGenTextures(1, &m_DepthTextureID);
	glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Framebuffer::AttachRenderBuffer(const GLuint& rbo_ID, const FramebufferAttachment& attachment)
{
	Bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(attachment), GL_RENDERBUFFER, rbo_ID);
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
