#include "Engine/Utils/pch.h"
#include "Mesh.h"

#include "Engine/OpenGL/VertexBufferLayout.h"

Mesh::Mesh(const std::string& name)
	: m_Name(name)
{
}

Mesh::Mesh(const std::string& name, const std::vector <Vertex>& vertices, const std::vector <GLuint>& indices)
	: m_Name(name), m_Vertices(vertices), m_Indices(indices)
{
	SetupMesh();
}

Mesh::~Mesh()
{

	delete m_VAO;
	delete m_IBO;
	delete m_VBO;
}

void Mesh::SetupMesh()
{
	m_VAO = new VertexArray();
	m_VBO = new VertexBuffer(m_Vertices);
	m_IBO = new IndexBuffer(m_Indices);
	
	m_VAO->AddIndexBuffer(*m_IBO);

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VAO->AddVertexBuffer(*m_VBO, layout);
}

void Mesh::SetMaterial(Material* material)
{
	m_Material = material;
}

Material* Mesh::GetMaterial()
{
	return m_Material;
}

void Mesh::SetName(const std::string& name)
{
	m_Name = name;
}

std::string Mesh::GetName()
{
	return m_Name;
}
