#include "Renderbuffer.h"

Renderbuffer::Renderbuffer(int width, int height)
{
	glGenRenderbuffers(1, &m_ID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 16, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_ID);
}

Renderbuffer::~Renderbuffer()
{
	//glDeleteRenderbuffers(1, &m_ID);
}

void Renderbuffer::Bind() const
{
	glBindRenderbuffer(GL_FRAMEBUFFER, m_ID);
}

void Renderbuffer::Unbind() const
{
	glBindRenderbuffer(GL_FRAMEBUFFER, 0);
}

GLuint Renderbuffer::getID() const
{
	return m_ID;
}

void Renderbuffer::setSampleSize(int samples)
{
	sampleSize = samples;
}
