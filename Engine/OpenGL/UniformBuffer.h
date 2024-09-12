#pragma once

#include <GL/glew.h>
class Shader;

class UniformBuffer
{
public:
	UniformBuffer(uint32 size, uint32 binding);
	~UniformBuffer();

	void Bind() const;
	void Unbind() const;
	
	void SetData(const void* data) const;
	void SetData(const void* data, uint32_t size, uint32_t offset);
	GLuint GetID() const { return m_ID; }

private:
	GLuint m_ID;
	uint32 m_SizeInBytes;
};