#pragma once
#include <queue>
#include "Types.h"

class EntityManager {
public:
	EntityManager();
	~EntityManager();

	Entity PopEntity();
	//1) KillEntity clears the live flag straight away so every IsEntityLive guard is correct the moment an entity dies
	void KillEntity(Entity);
	//2) while RecycleEntity does the structural work later to circumvent mid-iteration entity removal
	void RecycleEntity(Entity);	

	bool IsEntityLive(Entity) const;
	const Signature& GetEntSignature(Entity) const;
	void SetSignature(Entity, Signature);

private:
	std::queue<Entity> m_AvailableEntities;
	std::vector<Signature> m_EntSignatures;
	std::vector<bool> m_LiveEntities;
};