#pragma once
#include <queue>
#include "Types.h"

class EntityManager {
public:
	EntityManager();
	~EntityManager();

	Entity PopEntity();
	void PushEntity(Entity);
	const Signature& GetEntSignature(Entity) const;
	void SetSignature(Entity, Signature);

private:
	std::queue<Entity> m_AvailableEntities;
	std::vector<Signature> m_EntSignatures;
};