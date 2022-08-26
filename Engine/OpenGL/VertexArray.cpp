#include "Engine/Utils/pch.h"
#include "VertexArray.h"

#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::AddIndexBuffer(const IndexBuffer& ib)
{
	glVertexArrayElementBuffer(m_ID, ib.GetID());
}


void VertexArray::AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	glVertexArrayVertexBuffer(m_ID, 0, vb.GetID(), 0, layout.GetStride());

	const auto& elements = layout.GetElements();
	uint8_t offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) 
	{
		const auto& element = elements[i];
		glEnableVertexArrayAttrib(m_ID, i);
		glVertexArrayAttribFormat(m_ID, i, element.Count, element.Type, element.Normalized, offset);
		offset += element.Count * VertexBufferElement::GetSizeOfType(element.Type);
		glVertexArrayAttribBinding(m_ID, i, 0);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
