#include "Registry.h"

Registry::Registry() :
	m_EntManager(std::make_unique<EntityManager>()),
	m_CompManager(std::make_unique<ComponentManager>()),
	m_SysManager(std::make_unique<SystemManager>()),
	m_EventQ(std::make_unique<EventQueue>())
{}

Registry::Registry(Registry&& other) noexcept :
	m_EntManager(std::move(other.m_EntManager)),
	m_CompManager(std::move(other.m_CompManager)),
	m_SysManager(std::move(other.m_SysManager)),
	m_EventQ(std::move(other.m_EventQ))
{}

Entity Registry::CreateEntity() {
	return m_EntManager->PopEntity();
}

void Registry::DestroyEntity(Entity ent) {
	return m_EntManager->PushEntity(ent);
}

EventQueue& Registry::GetEventQueue() {
	return *m_EventQ;
}

void Registry::ResetManagers() {
	m_EntManager = std::make_unique<EntityManager>();
	m_CompManager = std::make_unique<ComponentManager>();
	m_SysManager = std::make_unique<SystemManager>();
	m_EventQ = std::make_unique<EventQueue>();
}