#include "Engine/Utils/pch.h"
#include "Cube.h"



Cube::Cube(const std::string& name, const float& w)
	: Mesh(name)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Studenten Anfang - Aufgabe 1
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	float w2 = w / 2;
	m_Vertices =
	{//			COORDINATES				//			Normals					//        Texture
		//Top Side
		{glm::vec3(-w2, w2, w2),		glm::vec3(0.0f,  1.0f, 0.0f),		glm::vec2(0.0f, 0.0f)},	//0
		{glm::vec3(w2, w2, w2),			glm::vec3(0.0f,  1.0f, 0.0f),		glm::vec2(0.0f, 1.0f)},	//1
		{glm::vec3(w2, w2, -w2),		glm::vec3(0.0f,  1.0f, 0.0f),		glm::vec2(1.0f, 1.0f)}, //2
		{glm::vec3(-w2, w2, -w2),		glm::vec3(0.0f,  1.0f, 0.0f),		glm::vec2(1.0f, 0.0f)}, //3

		//Bottom Side
		{glm::vec3(-w2, -w2, w2),		glm::vec3(0.0f,  -1.0f, 0.0f),		glm::vec2(0.0f, 0.0f)},	//4
		{glm::vec3(-w2, -w2, -w2),		glm::vec3(0.0f,  -1.0f, 0.0f),		glm::vec2(0.0f, 1.0f)},	//5
		{glm::vec3(w2, -w2, -w2),		glm::vec3(0.0f,  -1.0f, 0.0f),		glm::vec2(1.0f, 1.0f)},	//6
		{glm::vec3(w2, -w2, w2),		glm::vec3(0.0f,  -1.0f, 0.0f),		glm::vec2(1.0f, 0.0f)},	//7

		//Front Side
		{glm::vec3(-w2, -w2, w2),		glm::vec3(0.0f,  0.0f, 1.0f),		glm::vec2(0.0f, 0.0f)},	//8
		{glm::vec3(w2, -w2, w2),		glm::vec3(0.0f,  0.0f, 1.0f),		glm::vec2(0.0f, 1.0f)}, //9
		{glm::vec3(w2, w2, w2),		    glm::vec3(0.0f,  0.0f, 1.0f),		glm::vec2(1.0f, 1.0f)}, //10
		{glm::vec3(-w2, w2, w2),		glm::vec3(0.0f,  0.0f, 1.0f),		glm::vec2(1.0f, 0.0f)}, //11

		//Left Side
		{glm::vec3(-w2, -w2, w2),		glm::vec3(-1.0f,  0.0f, 0.0f),		glm::vec2(0.0f, 0.0f)},	//12
		{glm::vec3(-w2, w2, w2),		glm::vec3(-1.0f,  0.0f, 0.0f),		glm::vec2(0.0f, 1.0f)},	//13
		{glm::vec3(-w2, w2, -w2),		glm::vec3(-1.0f,  0.0f, 0.0f),		glm::vec2(1.0f, 1.0f)},	//14
		{glm::vec3(-w2, -w2, -w2),		glm::vec3(-1.0f,  0.0f, 0.0f),		glm::vec2(1.0f, 0.0f)},	//15

		//Right Side
		{glm::vec3(w2, -w2, w2),		glm::vec3(1.0f,  0.0f, 0.0f),		glm::vec2(0.0f, 0.0f)},	//16
		{glm::vec3(w2, -w2, -w2),		glm::vec3(1.0f,  0.0f, 0.0f),		glm::vec2(0.0f, 1.0f)},	//17
		{glm::vec3(w2, w2, -w2),		glm::vec3(1.0f,  0.0f, 0.0f),		glm::vec2(1.0f, 1.0f)},	//18
		{glm::vec3(w2, w2, w2),			glm::vec3(1.0f,  0.0f, 0.0f),		glm::vec2(1.0f, 0.0f)},	//19

		//Back Side
		{glm::vec3(-w2, -w2, -w2),		glm::vec3(0.0f,  0.0f, -1.0f),		glm::vec2(0.0f, 0.0f)},	//20
		{glm::vec3(-w2, w2, -w2),		glm::vec3(0.0f,  0.0f, -1.0f),		glm::vec2(0.0f, 1.0f)},	//21
		{glm::vec3(w2, w2, -w2),		glm::vec3(0.0f,  0.0f, -1.0f),		glm::vec2(1.0f, 1.0f)},	//22
		{glm::vec3(w2, -w2, -w2),		glm::vec3(0.0f,  0.0f, -1.0f),		glm::vec2(1.0f, 0.0f)},	//23

	};


	m_Indices =
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

	SetupMesh();
}

Cube::~Cube()
{
	m_Vertices.clear();
	m_Indices.clear();
}
