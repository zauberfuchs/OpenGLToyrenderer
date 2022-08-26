#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddIndexBuffer(const IndexBuffer& ib);
	void AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

private:
	GLuint m_ID;
};