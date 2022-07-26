#pragma once

#include "IMesh.h"
#include "ISceneObject.h"
#include "Material.h"

class Mesh: public IMesh
{
public:
	Mesh(const std::string& name);
	Mesh(const std::string& name, const std::vector <Vertex>& vertices, const std::vector <GLuint>& indices);
	~Mesh() override;

	void SetupMesh() override;
	
	void Draw(Transform& transform) override;

	void SetName(const std::string& name) override;
	std::string GetName() override;

	void SetMaterial(IMaterial* material) override;
	IMaterial* GetMaterial() override;


protected:
	std::string m_Name;
	std::vector <Vertex> m_Vertices;
	std::vector <GLuint> m_Indices;

	VertexArray* m_VAO;
	IndexBuffer* m_IBO;
	VertexBuffer* m_VBO;

	bool m_ShadowPass = false;
	uint16_t m_RenderMode = GL_TRIANGLES;

	IMaterial* m_Material = nullptr;
};
