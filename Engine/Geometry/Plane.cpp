#include "../../Engine/Utils/pch.h"
#include "Plane.h"

Plane::Plane(const std::string& name)
	: Mesh(name)
{
	m_Vertices = 
	{//			COORDINATES					//			Normals					//        Texture
		{glm::vec3(-0.5f,  0.0f, -0.5f),	 glm::vec3(0.0f,  1.0f,  0.0f),		glm::vec2(0.0f, 0.0f)},	// upper side
		{glm::vec3(0.5f,   0.0f, -0.5f),	 glm::vec3(0.0f,  1.0f,  0.0f),		glm::vec2(0.0f, 1.0f)},	// upper side
		{glm::vec3(0.5f,   0.0f,  0.5f),	 glm::vec3(0.0f,  1.0f,  0.0f),		glm::vec2(1.0f, 1.0f)},	// upper side
		{glm::vec3(-0.5f,  0.0f,  0.5f),	 glm::vec3(0.0f,  1.0f,  0.0f),		glm::vec2(1.0f, 0.0f)}	// upper side
	};

	m_Indices = 
	{  // note that we start from 0!
		0, 3, 2,				// 1.	triangle	bottom side
		2, 1, 0,				// 2.	triangle	bottom side
	};

	SetupMesh();
}