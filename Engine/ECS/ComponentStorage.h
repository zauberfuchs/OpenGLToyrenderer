#pragma once

#include "Engine/Utils/pch.h"

constexpr std::size_t MAX_ENTITIES = 10000;

using EntityID = std::size_t;

class Entity;
class EntityManager;

class ComponentStorageBase {
public:
	virtual ~ComponentStorageBase() = default;

	virtual void RemoveComponent(EntityID entityid) = 0;
	virtual void clear() = 0;
};

template <typename Component>
struct ComponentStorage : public ComponentStorageBase
{
	void AddComponent(EntityID entity_id, Component component) 
	{
		assert(m_EntityToIndexMap.find(entity_id) == m_EntityToIndexMap.end() && "Component added to same entity more than once.");

		// Put new entry at end
		size_t newIndex = m_Size;
		m_EntityToIndexMap[entity_id] = newIndex;
		m_IndexToEntityMap[newIndex] = entity_id;
		m_ComponentArray[newIndex] = component;
		++m_Size;
	}

	void RemoveComponent(EntityID entity_id) 
	{
		assert(m_EntityToIndexMap.find(entity_id) != m_EntityToIndexMap.end() && "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = m_EntityToIndexMap[entity_id];
		size_t indexOfLastElement = m_Size - 1;
		m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

		// Update map to point to moved spot
		EntityID entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
		m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		m_EntityToIndexMap.erase(entity_id);
		m_IndexToEntityMap.erase(indexOfLastElement);

		--m_Size;
	}

	Component& GetComponent(EntityID entity_id) 
	{
		assert(m_EntityToIndexMap.find(entity_id) != m_EntityToIndexMap.end() && "Retrieving non-existent component.");

		return m_ComponentArray[m_EntityToIndexMap[entity_id]];
	}
	
	
	bool HasComponent(EntityID entity_id) 
	{
		return m_EntityToIndexMap.find(entity_id) != m_EntityToIndexMap.end();
	}
	
	std::array<Component, MAX_ENTITIES>& GetComponentArray()
	{
		return m_ComponentArray;
	}
	
	std::unordered_map<EntityID, Component*> GetActiveComponents()
	{
		std::unordered_map<EntityID, Component*> activeComponents;
		for(int i = 0; i < m_Size; i++)
		{
			activeComponents.insert({ m_IndexToEntityMap.at(i), &m_ComponentArray.at(i) });
		}
		return activeComponents;
	}
	
	void clear() 
	{
		
	}
	
	std::array<Component, MAX_ENTITIES> m_ComponentArray;
	std::unordered_map<EntityID, size_t> m_EntityToIndexMap;
	std::unordered_map<size_t, EntityID> m_IndexToEntityMap;
	size_t m_Size{};
};