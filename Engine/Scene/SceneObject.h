#pragma once

#include "Engine/Components/Transform.h"
#include "Engine/ECS/Entity.h"

class Model;
class Animation;

class Light;

class SceneObject : std::enable_shared_from_this<SceneObject>
{
public:
	SceneObject(std::string name);
	~SceneObject();

	std::string GetName();
	
	void AddChildren(Ref<SceneObject> sceneObject);
	std::unordered_set<Ref<SceneObject>> GetChildren();

	void SetParent(SceneObject* sceneObject);
	SceneObject* GetParent() const;

	Animation* GetAnimation();
	void SetAnimation(Animation* anim);
	
	template <typename Component, typename... Args>
	void AddComponent(Args&&... args) 
	{
		m_Entity.AddComponent<Component>(std::forward<Args>(args)...);
	}
	
	template <typename Component>
	void RemoveComponent() 
	{
		m_Entity.RemoveComponent<Component>();
	}

	template <typename Component>
	auto& GetComponent() 
	{
		return m_Entity.GetComponent<Component>();
	}

	template <typename Component>
	bool HasComponent() const 
	{
		return m_Entity.HasComponent<Component>();
	}

private:
	std::string m_Name;

	SceneObject* m_Parent = nullptr; // just to observe the parent
	std::unordered_set<Ref<SceneObject>> m_Children;

	Animation* m_Animation = nullptr;
	
	Entity m_Entity;
};