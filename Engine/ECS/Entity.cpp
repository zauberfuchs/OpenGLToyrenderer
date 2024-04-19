#include "Entity.h"

Entity::Entity(EntityManager& manager, EntityID id)
	: manager(manager), id(id)
	{
	}
	
	Entity::operator EntityID() const 
	{
		return id;
	}