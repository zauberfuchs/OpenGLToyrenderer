#pragma once

#include <unordered_set>
#include <glm/glm.hpp>
#include "IModel.h"
#include "Transform.h"
#include "IShader.h"

class Animation;

struct Light;

class ISceneObject
{
public:
	virtual ~ISceneObject() = default;

	virtual std::string GetName() = 0;

	virtual void Render() = 0;

	virtual void AddModel(IModel* mesh) = 0;

	virtual void AddLight(Light* light) = 0;
	virtual Light* GetLight() = 0;

	virtual void AddChildren(ISceneObject* sceneObject) = 0;
	virtual std::unordered_set<ISceneObject*> GetChildren() = 0;

	virtual void SetParent(ISceneObject* sceneObject) = 0;
	virtual ISceneObject* GetParent() const = 0;

	virtual Animation* GetAnimation() = 0;
	virtual void SetAnimation(Animation* anim) = 0;
	
	
	virtual Transform* GetTransform() = 0;
	virtual IModel& GetModel() = 0;
};