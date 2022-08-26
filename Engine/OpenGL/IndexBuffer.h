#pragma once

#include <GL/glew.h>

class IndexBuffer
{
public:
	IndexBuffer(std::vector<GLuint>& indices);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void Delete() const;

	GLuint GetID() const { return m_ID; }

private:
	GLuint m_ID;
};