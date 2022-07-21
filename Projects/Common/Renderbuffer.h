#ifndef RENDERBUFFER_CLASS_H
#define RENDERBUFFER_CLASS_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

class Renderbuffer
{
private:
	GLuint m_ID;
	int sampleSize = 1;
public:
	Renderbuffer(int width, int height);
	~Renderbuffer();

	void Bind() const;
	void Unbind() const;
	GLuint getID() const;
	void setSampleSize(int samples);
};
#endif