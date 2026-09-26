#pragma once
#include <vector>
#include <algorithm>

#include "Types.h"

class Registry; //Forward declaration

class ISystem {
public:

	virtual ~ISystem() = default;

	void AddEntity(Entity ent) {
		if (!std::ranges::contains(m_Entities, ent)) {
			m_Entities.push_back(ent);
			m_EntitiesAppended = true;
		}
	}

	void RemoveEntity(Entity ent) {
		if (std::erase(m_Entities, ent) > 0) m_EntitiesRemoved = true;
	}

	const std::vector<Entity>& GetEntities() const {
		return m_Entities;
	}

	void SetRegistry(Registry* registry) {
		m_Registry = registry;
	}

protected:
	std::vector<Entity> m_Entities;
	Registry* m_Registry = nullptr;

	bool m_EntitiesAppended = false; 	//Set when an entity is appended, for systems that keep m_Entities in a sorted order.
	bool m_EntitiesRemoved = false; 	//Set when an entity leaves, for systems caching anything derived from m_Entities
};