#include "Engine/Utils/pch.h"
#include "Renderbuffer.h"

Renderbuffer::Renderbuffer()
{
	glGenRenderbuffers(1, &m_ID);
}

Renderbuffer::Renderbuffer(int width, int height)
{
	glGenRenderbuffers(1, &m_ID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 16, GL_DEPTH_COMPONENT24, width, height);
}

void Renderbuffer::CreateRenderBufferStorage(int width, int height, FramebufferTextureFormat format)
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_SampleSize, static_cast<GLenum>(format), width, height);
	
}

Renderbuffer::~Renderbuffer()
{
	glDeleteRenderbuffers(1, &m_ID);
}

void Renderbuffer::Bind() const
{
	glBindRenderbuffer(GL_FRAMEBUFFER, m_ID);
}

void Renderbuffer::Unbind() const
{
	glBindRenderbuffer(GL_FRAMEBUFFER, 0);
}

GLuint Renderbuffer::GetId() const
{
	return m_ID;
}

void Renderbuffer::SetSampleSize(int samples)
{
	m_SampleSize = samples;
}
