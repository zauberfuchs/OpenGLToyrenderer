#pragma once

#include "Engine/Components/Transform.h"

class Model;
class Animation;

class Light;

class SceneObject : std::enable_shared_from_this<SceneObject>
{
public:
	SceneObject(std::string name);
	~SceneObject();

	std::string GetName();

	void AddModel(Ref<Model> model);

	void AddLight(Light* light);
	Light* GetLight();
	
	void AddChildren(Ref<SceneObject> sceneObject);
	std::unordered_set<Ref<SceneObject>> GetChildren();

	void SetParent(SceneObject* sceneObject);
	SceneObject* GetParent() const;
	
	Transform* GetTransform();

	Animation* GetAnimation();
	void SetAnimation(Animation* anim);

	Ref<Model> GetModel();

private:
	std::string m_Name;

	SceneObject* m_Parent = nullptr; // just to observe the parent
	std::unordered_set<Ref<SceneObject>> m_Children;

	Transform* m_Transform = nullptr;

	Animation* m_Animation = nullptr;

	Light* m_Light = nullptr;
	Ref<Model> m_Model = nullptr;
};