#pragma once

#include "Engine/Utils/pch.h"

constexpr std::size_t MAX_ENTITIES = 10000;

using EntityID = std::size_t;

class Entity;
class EntityManager;

class ComponentStorageBase {
public:
	virtual ~ComponentStorageBase() = default;

	virtual void remove_component(EntityID entityid) = 0;
	virtual void clear() = 0;
};

template <typename Component>
struct ComponentStorage : public ComponentStorageBase
{

	std::unique_ptr<Component[]> data;
	//std::unique_ptr<std::array<Component,3>> data;
	std::size_t size = 0;
	std::size_t capacity = 0;


	void reserve(std::size_t new_capacity) {
		if (new_capacity > capacity) {
			std::unique_ptr<Component[]> new_data(new Component[new_capacity]);
			if (data) {
				std::memcpy(new_data.get(), data.get(), sizeof(Component) * size);
			}
			data = std::move(new_data);
			capacity = new_capacity;
		}
	}

	void add_component(EntityID entity_id, Component component) {
		reserve(entity_id + 1);
		data[entity_id] = std::move(component);
		size = std::max(size, entity_id + 1);
	}

	void remove_component(EntityID entity_id) {
		if (entity_id < size) {
			data[entity_id] = Component{};
		}
	}

	Component& get_component(EntityID entity_id) {
		assert(entity_id < size);
		return data[entity_id];
	}

	bool has_component(EntityID entity_id) const {
		return entity_id < size && data[entity_id] != Component{};
	}

	void clear() {
		data.reset();
		size = 0;
		capacity = 0;
	}
};