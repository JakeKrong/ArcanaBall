#include "EntityManager.h"

#include "assert.h"

EntityManager::EntityManager() {
	for (int i = 0; i < ENTITY_CAP; i++) {
		m_AvailableEntities.push(i);
	}
	m_EntSignatures.reserve(ENTITY_CAP);
};

EntityManager::~EntityManager() {};

Entity EntityManager::CreateEntity() {
	assert(m_AvailableEntities.size() != 0 && "No more available Entity ID's left!");

	Entity id = m_AvailableEntities.front();
	m_AvailableEntities.pop();

	return id;
}

void EntityManager::DestroyEntity(Entity ent) {
	m_AvailableEntities.push(ent);
	m_EntSignatures[ent].reset();
}

Signature& EntityManager::GetEntSignature(Entity ent) {
	return m_EntSignatures[ent];
}

void EntityManager::SetSignature(Entity ent, Signature sig) {
	m_EntSignatures[ent] = sig;
}