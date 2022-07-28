#pragma once

#include <GL/glew.h>

class Renderbuffer
{
public:
	Renderbuffer(int width, int height);
	~Renderbuffer();

	void Bind() const;
	void Unbind() const;
	GLuint GetId() const;
	void SetSampleSize(int samples);

private:
	GLuint m_ID;
	int m_SampleSize = 1;
};