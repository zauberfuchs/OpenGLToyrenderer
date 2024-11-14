#pragma once
#include <GL/glew.h>

#include "Engine/OpenGL/VertexArray.h"
#include "Engine/OpenGL/IndexBuffer.h"
#include "Engine/Components/Material.h"

class Mesh
{
public:
	Mesh();
	Mesh(std::vector<Vertex>vertices, std::vector<GLuint> indices);
	Mesh(std::vector<Vertex>vertices, std::vector<GLuint> indices, Ref<Material> material);
	virtual ~Mesh();

	static Mesh CreateUnitCube(const Material& material = Material());
	static Mesh CreateUnitSphere(const Material& material = Material(), uint16_t segments = 16);
//    static Mesh CreateUnitCubeInsides(const Material &material = Material());
//    static Mesh CreateHorizontalPlane(const Material &material = Material());


	void SetMaterial(Ref<Material> material) { m_Material = material; };

	Ref<Material> 		GetMaterial() 		{ return m_Material; };
	Ref<VertexArray> 	GetVAO() 			{ return m_VAO; }
	uint16_t 			GetRenderMode() 	{ return m_RenderMode; }
	uint32_t 			GetIndexCount() 	{ return m_IndexCount; }
	uint32_t 			GetVertexCount() 	{ return m_VertexCount; }
	
	void Draw();


protected:
	uint32	m_VertexCount;
    uint32 	m_IndexCount;
	
	Ref<VertexArray> 	m_VAO;
	Ref<IndexBuffer> 	m_IBO;
	Ref<VertexBuffer> 	m_VBO;
	
	uint16_t m_RenderMode = GL_TRIANGLES;

	Ref<Material> m_Material = nullptr;
};
