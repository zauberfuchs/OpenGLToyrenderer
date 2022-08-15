#pragma once

#include "Engine/Components/Transform.h"

class Model;
class Animation;

class Light;

class SceneObject 
{
public:
	SceneObject(std::string name);
	~SceneObject();

	std::string GetName();

	void AddModel(Model* model);

	void AddLight(Light* light);
	Light* GetLight();
	
	void AddChildren(SceneObject* sceneObject);
	std::unordered_set<SceneObject*> GetChildren();

	void SetParent(SceneObject* sceneObject);
	SceneObject* GetParent() const;
	
	Transform* GetTransform();

	Animation* GetAnimation();
	void SetAnimation(Animation* anim);

	Model& GetModel();

private:
	std::string m_Name;

	SceneObject* m_Parent = nullptr;
	std::unordered_set<SceneObject*> m_Children;

	Transform* m_Transform = nullptr;

	Animation* m_Animation = nullptr;

	Light* m_Light = nullptr;
	Model* m_Model = nullptr;
};