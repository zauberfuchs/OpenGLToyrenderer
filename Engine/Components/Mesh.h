#pragma once

#include "../../Engine/Interfaces/IMesh.h"

class Mesh: public IMesh
{
public:
	Mesh(const std::string& name);
	Mesh(const std::string& name, const std::vector <Vertex>& vertices, const std::vector <GLuint>& indices);
	~Mesh() override;

	void SetupMesh() override;

	void SetName(const std::string& name) override;
	std::string GetName() override;

	void SetMaterial(IMaterial* material) override;
	IMaterial* GetMaterial() override;

	VertexArray* GetVAO() override { return m_VAO; }
	uint16_t GetRenderMode() override { return m_RenderMode; }
	uint32_t GetIndicesSize() override { return static_cast<uint32_t>(m_Indices.size()); }



protected:
	std::string m_Name;
	std::vector <Vertex> m_Vertices;
	std::vector <GLuint> m_Indices;

	VertexArray* m_VAO;
	IndexBuffer* m_IBO;
	VertexBuffer* m_VBO;
	
	uint16_t m_RenderMode = GL_TRIANGLES;

	IMaterial* m_Material = nullptr;
};
