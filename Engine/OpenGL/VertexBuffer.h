#pragma once

#include <GL/glew.h>

// Structure to standardize the vertices used in the meshes
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexUV;
};

class VertexBuffer
{
public:
	VertexBuffer(std::vector<Vertex>& vertices);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	GLuint GetID() const { return m_ID; }

private:
	GLuint m_ID;
};