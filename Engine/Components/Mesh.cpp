#include "../../Engine/Utils/pch.h"
#include "Mesh.h"


#include "../../Engine/Scene/World.h"
#include "../../Engine/Components/Light.h"

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



	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VAO->AddBuffer(*m_VBO, layout);



	m_VAO->Unbind();
	m_VBO->Unbind();
	m_IBO->Unbind();
}

void Mesh::SetMaterial(IMaterial* material)
{
	m_Material = material;
}

IMaterial* Mesh::GetMaterial()
{
	return m_Material;
}

void Mesh::Draw(Transform& transform)
{
	m_VAO->Bind();
	m_Material->GetShader()->Bind();

	Light* l = World::Get().GetActiveScene()->GetSceneLightSources().begin()->second;
	m_Material->SetTexture(l->GetDepthmap());
	m_Material->GetShader()->SetUniformMat4f("model", transform.GetTransformMatrix());
	m_Material->GetShader()->SetUniformMat4f("lightSpaceMatrix", l->CreateLightSpaceMatrix());

	m_Material->Render();

	glDrawElements(m_RenderMode, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);

	m_Material->RenderPost();

	m_VAO->Unbind();
}

void Mesh::SetName(const std::string& name)
{
	m_Name = name;
}

std::string Mesh::GetName()
{
	return m_Name;
}
