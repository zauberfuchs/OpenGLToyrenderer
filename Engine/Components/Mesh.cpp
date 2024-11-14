#include "Engine/Utils/pch.h"
#include "Mesh.h"

#include "Engine/OpenGL/VertexBufferLayout.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Ref<Material> material)
: m_VertexCount((uint32)vertices.size()), m_IndexCount((uint32)indices.size()), m_Material(material)
{
	m_VAO = CreateRef<VertexArray>();
	m_VBO = CreateRef<VertexBuffer>(vertices);
	m_IBO = CreateRef<IndexBuffer>(indices);
	
	m_VAO->AddIndexBuffer(*m_IBO);

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VAO->AddVertexBuffer(*m_VBO, layout);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
: m_VertexCount((uint32)vertices.size()), m_IndexCount((uint32)indices.size())
{
	m_VAO = CreateRef<VertexArray>();
	m_VBO = CreateRef<VertexBuffer>(vertices);
	m_IBO = CreateRef<IndexBuffer>(indices);
	
	m_VAO->AddIndexBuffer(*m_IBO);

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VAO->AddVertexBuffer(*m_VBO, layout);
}

Mesh::~Mesh()
{
}

Mesh Mesh::CreateUnitCube(const Material &material) {
	float w2 = 1.0f / 2.0f;
	std::vector<Vertex> vertices =
	{//			COORDINATES				//			Normals					//        Texture
		//Top Side
		{glm::vec3(-w2, w2,  w2),		glm::vec3(0.0f,  1.0f, 0.0f),		glm::vec2(0.0f, 0.0f)},	//0
		{glm::vec3( w2, w2,  w2),		glm::vec3(0.0f,  1.0f, 0.0f),		glm::vec2(0.0f, 1.0f)},	//1
		{glm::vec3( w2, w2, -w2),		glm::vec3(0.0f,  1.0f, 0.0f),		glm::vec2(1.0f, 1.0f)}, //2
		{glm::vec3(-w2, w2, -w2),		glm::vec3(0.0f,  1.0f, 0.0f),		glm::vec2(1.0f, 0.0f)}, //3

		//Bottom Side
		{glm::vec3(-w2, -w2,  w2),		glm::vec3(0.0f,  -1.0f, 0.0f),		glm::vec2(0.0f, 0.0f)},	//4
		{glm::vec3(-w2, -w2, -w2),		glm::vec3(0.0f,  -1.0f, 0.0f),		glm::vec2(0.0f, 1.0f)},	//5
		{glm::vec3( w2, -w2, -w2),		glm::vec3(0.0f,  -1.0f, 0.0f),		glm::vec2(1.0f, 1.0f)},	//6
		{glm::vec3( w2, -w2,  w2),		glm::vec3(0.0f,  -1.0f, 0.0f),		glm::vec2(1.0f, 0.0f)},	//7

		//Front Side
		{glm::vec3(-w2, -w2, w2),		glm::vec3(0.0f,  0.0f, 1.0f),		glm::vec2(0.0f, 0.0f)},	//8
		{glm::vec3( w2, -w2, w2),		glm::vec3(0.0f,  0.0f, 1.0f),		glm::vec2(0.0f, 1.0f)}, //9
		{glm::vec3( w2,  w2, w2),		glm::vec3(0.0f,  0.0f, 1.0f),		glm::vec2(1.0f, 1.0f)}, //10
		{glm::vec3(-w2,  w2, w2),		glm::vec3(0.0f,  0.0f, 1.0f),		glm::vec2(1.0f, 0.0f)}, //11

		//Left Side
		{glm::vec3(-w2, -w2,  w2),		glm::vec3(-1.0f,  0.0f, 0.0f),		glm::vec2(0.0f, 0.0f)},	//12
		{glm::vec3(-w2,  w2,  w2),		glm::vec3(-1.0f,  0.0f, 0.0f),		glm::vec2(0.0f, 1.0f)},	//13
		{glm::vec3(-w2,  w2, -w2),		glm::vec3(-1.0f,  0.0f, 0.0f),		glm::vec2(1.0f, 1.0f)},	//14
		{glm::vec3(-w2, -w2, -w2),		glm::vec3(-1.0f,  0.0f, 0.0f),		glm::vec2(1.0f, 0.0f)},	//15

		//Right Side
		{glm::vec3(w2, -w2,  w2),		glm::vec3(1.0f,  0.0f, 0.0f),		glm::vec2(0.0f, 0.0f)},	//16
		{glm::vec3(w2, -w2, -w2),		glm::vec3(1.0f,  0.0f, 0.0f),		glm::vec2(0.0f, 1.0f)},	//17
		{glm::vec3(w2,  w2, -w2),		glm::vec3(1.0f,  0.0f, 0.0f),		glm::vec2(1.0f, 1.0f)},	//18
		{glm::vec3(w2,  w2,  w2),		glm::vec3(1.0f,  0.0f, 0.0f),		glm::vec2(1.0f, 0.0f)},	//19

		//Back Side
		{glm::vec3(-w2, -w2, -w2),		glm::vec3(0.0f,  0.0f, -1.0f),		glm::vec2(0.0f, 0.0f)},	//20
		{glm::vec3(-w2,  w2, -w2),		glm::vec3(0.0f,  0.0f, -1.0f),		glm::vec2(0.0f, 1.0f)},	//21
		{glm::vec3( w2,  w2, -w2),		glm::vec3(0.0f,  0.0f, -1.0f),		glm::vec2(1.0f, 1.0f)},	//22
		{glm::vec3( w2, -w2, -w2),		glm::vec3(0.0f,  0.0f, -1.0f),		glm::vec2(1.0f, 0.0f)},	//23
	};


	std::vector<GLuint> indices =
	{
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,
	};
	
	return Mesh(vertices, indices);
}

Mesh Mesh::CreateUnitSphere(const Material &material, uint16_t segments)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;
	std::vector<Vertex> vertices;

    const unsigned int X_SEGMENTS = segments;
    const unsigned int Y_SEGMENTS = segments;
    const float PI = 3.14159265359f;

    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
    {
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
            normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = true;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }

    std::vector<float> data;
    for (unsigned int i = 0; i < positions.size(); ++i)
    {
        vertices.push_back({ positions[i], normals[i], uv[i] });
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        if (normals.size() > 0)
        {
            data.push_back(normals[i].x);
            data.push_back(normals[i].y);
            data.push_back(normals[i].z);
        }
        if (uv.size() > 0)
        {
            data.push_back(uv[i].x);
            data.push_back(uv[i].y);
        }
    }
	Mesh mesh = Mesh(vertices, indices);
	mesh.m_RenderMode = GL_TRIANGLE_STRIP;
	return mesh;
}

void Mesh::Draw()
{
	return glDrawElements(m_RenderMode, m_IndexCount, GL_UNSIGNED_INT, nullptr);
}
