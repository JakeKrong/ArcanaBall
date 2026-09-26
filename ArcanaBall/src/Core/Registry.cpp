#include "Registry.h"

Registry::Registry() :
	m_EntManager(std::make_unique<EntityManager>()),
	m_CompManager(std::make_unique<ComponentManager>()),
	m_SysManager(std::make_unique<SystemManager>()),
	m_EventQ(std::make_unique<EventQueue>())
{}

Entity Registry::CreateEntity() {
	return m_EntManager->PopEntity();
}

//Kill now, reap later. The live flag drops immediately so IsEntityLive is correct for the
//rest of the frame, but components stay readable and m_Entities stays put until ReapDestroyed
void Registry::DestroyEntity(Entity ent) {
	//Ignore repeat destroys, they would queue the same ID for reaping twice
	if (!m_EntManager->IsEntityLive(ent)) return;

	m_EntManager->KillEntity(ent);
	m_PendingReap.push_back(ent);
}

//Strip components, drop the entity from every system, and return the ID to the pool
void Registry::ReapDestroyed() {
	for (Entity ent : m_PendingReap) {
		//Read the signature before RecycleEntity resets it
		Signature sig = m_EntManager->GetEntSignature(ent);

		m_CompManager->DestroyEntComponents(ent, sig);
		m_SysManager->EntityDestroyed(ent, sig);
		m_EntManager->RecycleEntity(ent);
	}
	m_PendingReap.clear();
}

bool Registry::IsEntityLive(Entity ent) const {
	return m_EntManager->IsEntityLive(ent);
}

EventQueue& Registry::GetEventQueue() {
	return *m_EventQ;
}

void Registry::ResetManagers() {
	m_PendingReap.clear();
	m_EntManager = std::make_unique<EntityManager>();
	m_CompManager = std::make_unique<ComponentManager>();
	m_SysManager = std::make_unique<SystemManager>();
	m_EventQ = std::make_unique<EventQueue>();
}