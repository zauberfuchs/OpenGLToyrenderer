#include "EntityManager.h"

	EntityManager::EntityManager() 
	{
		entity_mask.reset();
	}

	Entity EntityManager::create_entity() 
	{
		assert(entity_count < MAX_ENTITIES);
		EntityID id = entity_count++;
		entity_mask[id] = true;
		return Entity(*this, id);
	}

	void EntityManager::destroy_entity(EntityID entity_id) 
	{
		assert(entity_id < entity_count);
		for (auto& storage : componentStorages) 
		{
			storage.second->RemoveComponent(entity_id);
		}
		entity_mask[entity_id] = false;
	}

	void EntityManager::clear() {
		entity_count = 0;
		entity_mask.reset();
		for (auto& storage : componentStorages) {
			storage.second->clear();
		}
	}