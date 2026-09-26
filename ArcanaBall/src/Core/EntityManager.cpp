#include "EntityManager.h"

#include "assert.h"
#include <stdexcept>

EntityManager::EntityManager() {
	for (int i = 1; i <= ENTITY_CAP; i++) {
		m_AvailableEntities.push(i);
	}
	m_EntSignatures.resize(ENTITY_CAP + 1);
	m_LiveEntities.resize(ENTITY_CAP + 1, false);
};

EntityManager::~EntityManager() {};

Entity EntityManager::PopEntity() {
	assert(m_AvailableEntities.size() != 0 && "No more available Entity ID's left!");

	if (m_AvailableEntities.empty()) throw std::runtime_error("EntityManager: no available Entity IDs left!");

	Entity id = m_AvailableEntities.front();
	m_AvailableEntities.pop();
	m_LiveEntities[id] = true;

	return id;
}

void EntityManager::KillEntity(Entity ent) {
	m_LiveEntities[ent] = false;
}

void EntityManager::RecycleEntity(Entity ent) {
	m_EntSignatures[ent].reset();
	m_AvailableEntities.push(ent);
}

bool EntityManager::IsEntityLive(Entity ent) const {
	return ent < m_LiveEntities.size() && m_LiveEntities[ent];
}

const Signature& EntityManager::GetEntSignature (Entity ent) const {
	return m_EntSignatures[ent];
}

void EntityManager::SetSignature(Entity ent, Signature sig) {
	m_EntSignatures[ent] = sig;
}
