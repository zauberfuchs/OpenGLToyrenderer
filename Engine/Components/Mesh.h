#pragma once
#include <GL/glew.h>

#include "Engine/OpenGL/VertexArray.h"
#include "Engine/OpenGL/IndexBuffer.h"
#include "Engine/Components/Material.h"

class Mesh
{
public:
	Mesh(const std::string& name);
	Mesh(const std::string& name, const std::vector <Vertex>& vertices, const std::vector <GLuint>& indices);
	virtual ~Mesh();

	void SetupMesh();

	void SetName(const std::string& name);
	std::string GetName();

	void SetMaterial(Material* material);
	Material* GetMaterial();

	VertexArray* GetVAO() { return m_VAO; }
	uint16_t GetRenderMode() { return m_RenderMode; }
	uint32_t GetIndicesSize() { return static_cast<uint32_t>(m_Indices.size()); }



protected:
	std::string m_Name;
	std::vector <Vertex> m_Vertices;
	std::vector <GLuint> m_Indices;

	VertexArray* m_VAO;
	IndexBuffer* m_IBO;
	VertexBuffer* m_VBO;
	
	uint16_t m_RenderMode = GL_TRIANGLES;

	Material* m_Material = nullptr;
};
