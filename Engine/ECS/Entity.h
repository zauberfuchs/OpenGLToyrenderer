#pragma once

#include "Engine/Utils/pch.h"

#include "EntityManager.h";
#include "ComponentStorage.h";

class Entity 
{
public:
	Entity(EntityManager& manager, EntityID id);
	
	template <typename Component, typename... Args>
	void add_component(Args&&... args);

	template <typename Component>
	void remove_component();

	template <typename Component>
	Component& get_component();

	template <typename Component>
	bool has_component() const;

	operator EntityID() const;

private:
	EntityManager& manager;
	EntityID id;
};