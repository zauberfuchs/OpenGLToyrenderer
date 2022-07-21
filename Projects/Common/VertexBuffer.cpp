#include "VertexBuffer.h"

// kann erweitert werden mit set data / lock / unlock damit man die daten modifizieren kann
VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices)
{
	assert(!vertices.empty());
	glGenBuffers(1, &m_ID); // in buffer wird die BufferId geschrieben die der buffer bekommt.
	glBindBuffer(GL_ARRAY_BUFFER, m_ID); // ich binde mein buffer mit dem GL_ARRAY_BUFFER!
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); // buffer array wird erstellt/befüllt und die daten an die GPU gesendet!
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
