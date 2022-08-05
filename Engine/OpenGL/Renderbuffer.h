#pragma once

#include <GL/glew.h>

#include "Framebuffer.h"

class Renderbuffer
{
public:
	Renderbuffer();
	Renderbuffer(int width, int height);
	~Renderbuffer();

	void CreateRenderBufferStorage(int width, int height, FramebufferTextureFormat format);
	void Bind() const;
	void Unbind() const;
	GLuint GetId() const;
	void SetSampleSize(int samples);

private:
	GLuint m_ID;
	int m_SampleSize = 1;
};
