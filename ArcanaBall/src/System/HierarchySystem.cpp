#include "HierarchySystem.h"
#include "Child.h"
#include "Registry.h"
#include "EntityDestroyedEvent.h"

void HierarchySystem::Update() {
	auto& childCompArr = m_Registry->GetComponentArray<Child>();
	auto& transCompArr = m_Registry->GetComponentArray<Transform>();

	for (Entity ent : m_Entities) {
		if (!m_Registry->IsEntityLive(ent)) continue;

		auto& childTrans = transCompArr.GetTComponent(ent);
		Entity parentEnt = childCompArr.GetTComponent(ent).parentEntity;
		if (m_Registry->IsEntityLive(parentEnt)) {
			Transform& parentTrans = transCompArr.GetTComponent(parentEnt);
			childTrans.position = parentTrans.position + childCompArr.GetTComponent(ent).localOffset;
		}
		else {
			m_Registry->DestroyEntity(ent);
		}
	}
}

void HierarchySystem::ProcessDestroyRequests() {
	auto& childCompArr = m_Registry->GetComponentArray<Child>();

	for (auto event : m_Registry->GetEventQueue().GetTEvents<DestroyChildEntity>()) {
		for (Entity ent : m_Entities) {
			if (m_Registry->IsEntityLive(ent) && childCompArr.GetTComponent(ent).parentEntity == event->parentEntity) {
				m_Registry->DestroyEntity(ent);
			}
		}
	}
}