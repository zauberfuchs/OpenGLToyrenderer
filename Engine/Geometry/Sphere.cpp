//#include "Engine/Utils/pch.h"
//#include "Sphere.h"
//
//Sphere::Sphere(const std::string& name, const uint16_t& segments)
//	: Mesh(name)
//{
//    m_RenderMode = GL_TRIANGLE_STRIP;
//    std::vector<glm::vec3> positions;
//    std::vector<glm::vec2> uv;
//    std::vector<glm::vec3> normals;
//
//    const unsigned int X_SEGMENTS = segments;
//    const unsigned int Y_SEGMENTS = segments;
//    const float PI = 3.14159265359f;
//
//    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
//    {
//        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
//        {
//            float xSegment = (float)x / (float)X_SEGMENTS;
//            float ySegment = (float)y / (float)Y_SEGMENTS;
//            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
//            float yPos = std::cos(ySegment * PI);
//            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
//
//            positions.push_back(glm::vec3(xPos, yPos, zPos));
//            uv.push_back(glm::vec2(xSegment, ySegment));
//            normals.push_back(glm::vec3(xPos, yPos, zPos));
//        }
//    }
//
//    bool oddRow = true;
//    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
//    {
//        if (!oddRow) // even rows: y == 0, y == 2; and so on
//        {
//            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
//            {
//                m_Indices.push_back(y * (X_SEGMENTS + 1) + x);
//                m_Indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
//            }
//        }
//        else
//        {
//            for (int x = X_SEGMENTS; x >= 0; --x)
//            {
//                m_Indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
//                m_Indices.push_back(y * (X_SEGMENTS + 1) + x);
//            }
//        }
//        oddRow = !oddRow;
//    }
//
//    std::vector<float> data;
//    for (unsigned int i = 0; i < positions.size(); ++i)
//    {
//        m_Vertices.push_back({ positions[i], normals[i], uv[i] });
//        data.push_back(positions[i].x);
//        data.push_back(positions[i].y);
//        data.push_back(positions[i].z);
//        if (normals.size() > 0)
//        {
//            data.push_back(normals[i].x);
//            data.push_back(normals[i].y);
//            data.push_back(normals[i].z);
//        }
//        if (uv.size() > 0)
//        {
//            data.push_back(uv[i].x);
//            data.push_back(uv[i].y);
//        }
//    }
//	SetupMesh();
//}
//
//Sphere::~Sphere()
//{
//	m_Vertices.clear();
//	m_Indices.clear();
//}
