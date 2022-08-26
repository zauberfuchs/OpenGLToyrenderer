#include "Engine/Utils/pch.h"
#include "VertexBuffer.h"

// kann erweitert werden mit set data / lock / unlock damit man die daten modifizieren kann
VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices)
{
	assert(!vertices.empty());

	glCreateBuffers(1, &m_ID);
	glNamedBufferData(m_ID, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}


VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
