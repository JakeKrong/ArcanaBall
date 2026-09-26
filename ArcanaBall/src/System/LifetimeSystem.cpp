#include "LifetimeSystem.h"
#include "Registry.h"

void LifetimeSystem::Update() {
	if (!m_Registry) return;

	auto& lifetimeCompArr = m_Registry->GetComponentArray<Lifetime>();

	for (Entity ent : m_Entities) {
		if (!m_Registry->IsEntityLive(ent)) continue; //Awaiting reap

		Lifetime& lifetime = lifetimeCompArr.GetTComponent(ent);

		if (--lifetime.collisionPasses <= 0) {
			m_Registry->DestroyEntity(ent);
		}
	}
}
