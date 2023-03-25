#pragma once

#include "Engine/Utils/pch.h"

#include "EntityManager.h"
#include "ComponentStorage.h"

class EntityManager;

template <typename T, typename ... Args>
T createT(Args&& ... args){
	return T(std::forward<Args>(args) ... );
}

struct Velocity 
{
	//Velocity(float x, float y, float z) {}
	float x;
	float y;
	float z;
};

class Entity 
{
public:
	
	//template <typename Component, typename... Args>
	//void add_component(Args&&... args);

	template <typename Component, typename ... Args>
	void add_component(Args&& ... args)
	{
		EntityManager* entity_manager = new EntityManager();
		Velocity v;
		EntityID id = 2;
		//manager.add_component<Component>(id, Component(std::forward<Args>(args)...));
		entity_manager->add_component<Velocity>(id, v);
		//return T(std::forward<Args>(args) ... );
	}

	template <typename Component>
	void remove_component();

	template <typename Component>
	Component& get_component();

	template <typename Component>
	bool has_component() const;

	operator EntityID() const;

private:
	friend EntityManager;
	Entity(EntityManager& manager, EntityID id);

	EntityManager& manager;
	EntityID id;
};