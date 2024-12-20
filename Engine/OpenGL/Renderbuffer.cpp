#include "Engine/Utils/pch.h"
#include "Renderbuffer.h"

Renderbuffer::Renderbuffer()
{
	glCreateRenderbuffers(1, &m_ID);
}

void Renderbuffer::CreateRenderBufferStorage(int width, int height, FramebufferTextureFormat format)
{
	glNamedRenderbufferStorageMultisample(m_ID, m_SampleSize, static_cast<GLenum>(format), width, height);

}

Renderbuffer::~Renderbuffer()
{
	glDeleteRenderbuffers(1, &m_ID);
}

void Renderbuffer::Bind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
}

void Renderbuffer::Unbind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GLuint Renderbuffer::GetId() const
{
	return m_ID;
}

void Renderbuffer::SetSampleSize(int samples)
{
	m_SampleSize = samples;
}
