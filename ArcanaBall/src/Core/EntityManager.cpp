#include "EntityManager.h"

#include "assert.h"

EntityManager::EntityManager() {
	for (int i = 0; i < ENTITY_CAP; i++) {
		m_AvailableEntities.push(i);
	}
	m_EntSignatures.resize(ENTITY_CAP);
};

EntityManager::~EntityManager() {};

Entity EntityManager::PopEntity() {
	assert(m_AvailableEntities.size() != 0 && "No more available Entity ID's left!");

	Entity id = m_AvailableEntities.front();
	m_AvailableEntities.pop();

	return id;
}

void EntityManager::PushEntity(Entity ent) {
	m_AvailableEntities.push(ent);
	m_EntSignatures[ent].reset();
}

Signature EntityManager::GetEntSignature(Entity ent) {
	return m_EntSignatures[ent];
}

void EntityManager::SetSignature(Entity ent, Signature sig) {
	m_EntSignatures[ent] = sig;
}