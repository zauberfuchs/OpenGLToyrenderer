#include "Engine/Utils/pch.h"
#include "IndexBuffer.h"

#include <cassert>


IndexBuffer::IndexBuffer(std::vector<GLuint>& indices)
{
	assert(!indices.empty());

	glCreateBuffers(1, &m_ID);
	glNamedBufferData(m_ID, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
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

