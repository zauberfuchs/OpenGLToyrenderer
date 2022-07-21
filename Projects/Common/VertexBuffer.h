#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

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
	virtual ~VertexBuffer();

	void Bind() const;
	void Unbind() const;

private:
	GLuint m_ID;
};