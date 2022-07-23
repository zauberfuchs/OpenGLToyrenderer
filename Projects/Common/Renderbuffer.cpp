#include "Renderbuffer.h"

Renderbuffer::Renderbuffer(int width, int height)
{
	glGenRenderbuffers(1, &m_ID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
//	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 16, GL_DEPTH24_STENCIL8, width, height);
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
