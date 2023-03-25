#include "EntityManager.h"

	EntityManager::EntityManager() {
		entity_mask.fill(false);
	}

	Entity EntityManager::create_entity() {
		assert(entity_count < MAX_ENTITIES);
		EntityID id = entity_count++;
		return Entity(*this, id);
	}

	void EntityManager::destroy_entity(EntityID entity_id) {
		assert(entity_id < entity_count);
		for (auto& storage : component_storages) {
			storage.second.remove_component(entity_id);
		}
		entity_mask[entity_id] = false;
	}

	template <typename Component>
	void EntityManager::add_component(EntityID entity_id, Component component) {
		component_storage<Component>().add_component(entity_id, std::move(component));
		entity_mask[entity_id] = true;
	}

	template <typename Component>
	void EntityManager::remove_component(EntityID entity_id) {
		component_storage<Component>().remove_component(entity_id);
		entity_mask[entity_id] = any_component(entity_mask());
	}
	template <typename Component>
	Component& EntityManager::get_component(EntityID entity_id) {
		return component_storage<Component>().get_component(entity_id);
	}

	template <typename Component>
	bool EntityManager::has_component(EntityID entity_id) const {
		return component_storage<Component>().has_component(entity_id);
	}

	void EntityManager::clear() {
		entity_count = 0;
		entity_mask.fill(false);
		for (auto& storage : component_storages) {
			storage.second.clear();
		}
	}

	template <typename Component>
	ComponentStorage<Component>& EntityManager::component_storage() {
		std::size_t type_id = typeid(Component).hash_code();
		auto it = component_storages.find(type_id);
		if (it == component_storages.end()) {
			component_storages[type_id] = std::unique_ptr<ComponentStorage<Component>, void(*)(void*)>(
				new ComponentStorage<Component>(), [](void* ptr) { delete static_cast<ComponentStorage<Component>*>(ptr); });
			it = component_storages.find(type_id);
		}
		return *static_cast<ComponentStorage<Component>*>(it->second.get());
	}

	std::bitset<MAX_ENTITIES> EntityManager::any_component_mask() const {
		std::bitset<MAX_ENTITIES> mask;
		for (const auto& storage : component_storages) {
			mask |= component_mask(storage.first);
		}
		return mask;
	}

	std::bitset<MAX_ENTITIES> EntityManager::component_mask(std::size_t type_id) const {
		auto it = component_storages.find(type_id);
		if (it == component_storages.end()) {
			return std::bitset<MAX_ENTITIES>{};
		} else {
			const auto& storage = *static_cast<const ComponentStorageBase*>(it->second.get());
			std::bitset<MAX_ENTITIES> mask;
			for (std::size_t i = 0; i < storage.size; ++i) {
				if (storage.has_component(i)) {
					mask[i] = true;
				}
			}
			return mask;
		}
	}
