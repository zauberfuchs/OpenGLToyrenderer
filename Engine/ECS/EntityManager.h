#pragma once

#include "Engine/Utils/pch.h"

#include "ComponentStorage.h"
#include "Entity.h"

class EntityManager {
public:
	EntityManager();

	Entity create_entity();

	void destroy_entity(EntityID entity_id);

	template <typename Component>
	void add_component(EntityID entity_id, Component component);

	template <typename Component>
	void remove_component(EntityID entity_id);

	template <typename Component>
	Component& get_component(EntityID entity_id);

	template <typename Component>
	bool has_component(EntityID entity_id) const;

	void clear();

	
private:
	std::size_t entity_count = 0;
	std::bitset<MAX_ENTITIES> entity_mask;

	std::unordered_map<std::size_t, std::unique_ptr<ComponentStorageBase>> component_storages;
	std::unordered_map<std::type_index, std::unique_ptr<ComponentStorageBase>> component_storages;

	EntityID next_entity_id;

	template <typename Component>
	ComponentStorage<Component>& component_storage();

	std::bitset<MAX_ENTITIES> any_component_mask() const;

	std::bitset<MAX_ENTITIES> component_mask(std::size_t type_id) const;
};