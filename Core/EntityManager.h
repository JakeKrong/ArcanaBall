#pragma once
#include <queue>
#include "Types.h"

class EntityManager {
public:
	EntityManager();
	~EntityManager();

	Entity CreateEntity();
	void DestroyEntity(Entity);
	Signature& GetEntSignature(Entity);
	void SetSignature(Entity, Signature);

private:
	std::queue<Entity> m_AvailableEntities;
	std::vector<Signature> m_EntSignatures;
};