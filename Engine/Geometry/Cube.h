#pragma once

#include "Engine/Components/Mesh.h" 

class Cube : public Mesh
{
public:
	Cube(const std::string& name, const float& w=1.0f);
	~Cube();
};