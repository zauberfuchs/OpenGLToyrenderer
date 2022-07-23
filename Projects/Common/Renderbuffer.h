#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

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