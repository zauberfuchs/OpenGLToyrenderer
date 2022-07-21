#pragma once

#include <GL/glew.h>
#include <vector>

class IndexBuffer
{
private:
	GLuint m_ID;
public:
	IndexBuffer(std::vector<GLuint>& indices);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void Delete() const;
};