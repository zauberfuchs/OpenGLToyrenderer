#pragma once

#include "../../Engine/Interfaces/ICube.h"
#include "../../Engine/Components/Mesh.h" 

class StudentCube : public ICube, public Mesh
{
public:
	StudentCube(const std::string& name, const float& w=1.0f);
	virtual ~StudentCube() override;

};