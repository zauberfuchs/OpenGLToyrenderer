#pragma once

#include "ICube.h"
#include "Mesh.h" 

class StudentCube : public ICube, public Mesh
{
public:
	StudentCube(const std::string& name, const float& w=1.0f);
	virtual ~StudentCube() override;

};