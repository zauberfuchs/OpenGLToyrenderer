#pragma once

#include "ICube.h"
#include "Mesh.h" 

class Sphere : public Mesh
{
public:
	Sphere(const std::string& name, const uint16_t& segments = 64);
	virtual ~Sphere() override;

};