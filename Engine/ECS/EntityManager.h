#pragma once

#include "Engine/Utils/pch.h"

#include "ComponentStorage.h"
#include "Entity.h"


class EntityManager 
{
public:
	EntityManager();

	Entity create_entity();

	void destroy_entity(EntityID entity_id);

	template <typename Component>
	void AddComponent(EntityID entity_id, Component component) 
	{
		GetComponentStorage<Component>()->AddComponent(entity_id, std::move(component));
	}

	template <typename Component>
	void RemoveComponent(EntityID entity_id) 
	{
		GetComponentStorage<Component>()->RemoveComponent(entity_id);
	}
	
	template <typename Component>
	auto& GetComponent(EntityID entity_id) 
	{
		return GetComponentStorage<Component>()->GetComponent(entity_id);
	}

	template <typename Component>
	bool HasComponent(EntityID entity_id) 
	{
		return GetComponentStorage<Component>()->HasComponent(entity_id);
	}
	
	template <typename Component>
	auto& GetComponents()
	{
		return GetComponentStorage<Component>()->GetComponentArray();
	}
	
	template <typename Component>
	auto& GetActiveComponentCount()
	{
		return GetComponentStorage<Component>()->m_Size;
	}
	
	template <typename Component>
	auto GetActiveComponents()
	{
		return GetComponentStorage<Component>()->GetActiveComponents();
	}

	void clear();
	
private:
	template <typename Component>
	ComponentStorage<Component>* GetComponentStorage() 
	{
		std::size_t type_id = typeid(Component).hash_code();
		auto it = componentStorages.find(type_id);
		if (it != componentStorages.end()) {
			return static_cast<ComponentStorage<Component>*>(it->second.get());
		}
		else
		{
			// Create a new ComponentStorage object and store it in a unique_ptr
			std::unique_ptr<ComponentStorageBase> storage = std::make_unique<ComponentStorage<Component>>();
			// Insert the new unique_ptr into the component_storages map
			auto result = componentStorages.emplace(type_id, std::move(storage));
			return static_cast<ComponentStorage<Component>*>(result.first->second.get());
		}
	};


private:

	std::size_t entity_count = 0;
	std::bitset<MAX_ENTITIES> entity_mask;

	std::unordered_map<std::size_t, std::unique_ptr<ComponentStorageBase>> componentStorages;

	EntityID next_entity_id;
};