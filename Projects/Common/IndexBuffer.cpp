#include "IndexBuffer.h"

#include <cassert>


IndexBuffer::IndexBuffer(std::vector<GLuint>& indices)
{
	assert(!indices.empty());
	glGenBuffers(1, &m_ID); // in buffer wird die BufferId geschrieben die der buffer bekommt.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID); // ich binde mein buffer mit dem GL_ARRAY_BUFFER!
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW); // buffer array wird erstellt/befüllt und die daten an die GPU gesendet!
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void IndexBuffer::Bind() const 
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Delete() const
{
	glDeleteBuffers(1, &m_ID);
}

