#include "Entity.h"

Entity::Entity(EntityManager& manager, EntityID id)
	: manager(manager), id(id)
	{
	}
	
	//template <typename Component, typename... Args>
	//void Entity::add_component(Args&&... args) 
	//{
	//	//manager.add_component<Component>(id, Component{std::forward<Args>(args)...});
	//}

	template <typename Component>
	void Entity::remove_component() 
	{
		manager.remove_component<Component>(id);
	}

	template <typename Component>
	Component& Entity::get_component() 
	{
		return manager.get_component<Component>(id);
	}

	template <typename Component>
	bool Entity::has_component() const 
	{
		return manager.has_component<Component>(id);
	}

	Entity::operator EntityID() const 
	{
		return id;
	}