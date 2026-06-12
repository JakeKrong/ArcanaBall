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
		}
	}

	void RemoveEntity(Entity ent) {
		std::erase(m_Entities, ent);
	}

	std::vector<Entity> returnEntities() {
		return m_Entities;
	}

	void SetRegistry(Registry* registry) {
		m_Registry = registry;
	}

protected:
	std::vector<Entity> m_Entities;
	Registry* m_Registry = nullptr;
};