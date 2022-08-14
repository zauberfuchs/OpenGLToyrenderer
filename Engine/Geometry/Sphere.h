#pragma once

#include "Engine/Components/Mesh.h" 

class Sphere : public Mesh
{
public:
	Sphere(const std::string& name, const uint16_t& segments = 64);
	~Sphere();
};