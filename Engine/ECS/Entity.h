#pragma once

#include "Engine/Utils/pch.h"

#include "EntityManager.h"
#include "ComponentStorage.h"


class Entity 
{
public:
	template <typename Component, typename... Args>
	void AddComponent(Args&&... args) 
	{
		manager.AddComponent<Component>(id, Component{std::forward<Args>(args)...});
	}
	
	template <typename Component>
	void RemoveComponent() 
	{
		manager.RemoveComponent<Component>(id);
	}

	template <typename Component>
	auto& GetComponent() 
	{
		return manager.GetComponent<Component>(id);
	}

	template <typename Component>
	bool HasComponent() const 
	{
		return manager.HasComponent<Component>(id);
	}

	operator EntityID() const;

private:
	friend EntityManager;
	Entity(EntityManager& manager, EntityID id);

	EntityManager& manager;
	EntityID id;
};