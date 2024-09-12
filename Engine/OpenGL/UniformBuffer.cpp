#include "Engine/Utils/pch.h"
#include "UniformBuffer.h"
#include "Shader.h"

UniformBuffer::UniformBuffer(uint32 size, uint32 binding)
	: m_SizeInBytes(size)
{
	glCreateBuffers(1, &m_ID);
	glNamedBufferData(m_ID, m_SizeInBytes, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID);
}

void UniformBuffer::SetData(const void* data) const
{
	glNamedBufferData(m_ID, m_SizeInBytes, data, GL_STATIC_DRAW);
}

void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
{
	glNamedBufferSubData(m_ID, offset, size, data);
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void UniformBuffer::Bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
}

void UniformBuffer::Unbind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}