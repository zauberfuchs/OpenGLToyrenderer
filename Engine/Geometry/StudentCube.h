#pragma once

#include "Engine/Components/Mesh.h" 

class StudentCube : public Mesh
{
public:
	StudentCube(const std::string& name, const float& w=1.0f);
	~StudentCube();
};