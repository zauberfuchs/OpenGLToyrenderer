#pragma once

#include <unordered_map>

#include "Camera.h"
#include "Transform.h"
#include <vector>

class IMesh;

class IModel
{
public:
	virtual ~IModel() {};
	virtual void Draw(Transform& transform) = 0;

	virtual void AddMesh(IMesh* mesh) = 0;
	virtual IMesh* GetMesh(const int& index) = 0;
	virtual IMesh* GetMesh(const std::string& name) = 0;
	virtual std::unordered_map<std::string, IMesh*> GetMeshes() = 0;
};