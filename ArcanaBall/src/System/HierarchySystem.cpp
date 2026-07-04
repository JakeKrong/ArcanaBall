#include "HierarchySystem.h"
#include "Child.h"
#include "Registry.h"
#include "EntityDestroyedEvent.h"

void HierarchySystem::Update() {
	auto& childCompArr = m_Registry->GetComponentArray<Child>();
	auto& transCompArr = m_Registry->GetComponentArray<Transform>();

	auto test = m_Registry->GetEventQueue().GetTEvents<DestroyChildEntity>();
	for (auto& event : m_Registry->GetEventQueue().GetTEvents<DestroyChildEntity>()) {
		for (Entity ent : m_Entities) {
			if (m_Registry->EntityHasComponent<Child>(ent) && 
				childCompArr.GetTComponent(ent).parentEntity == event->parentEntity) {
				m_Registry->DestroyEntity(ent);
			}
		}
	}

	for (Entity ent : m_Entities) {
		auto& childTrans = transCompArr.GetTComponent(ent);
		Entity parentEnt = childCompArr.GetTComponent(ent).parentEntity;
		if (m_Registry->EntityHasComponent<Transform>(parentEnt)) {
			Transform& parentTrans = transCompArr.GetTComponent(parentEnt);
			childTrans.position = parentTrans.position + childCompArr.GetTComponent(ent).localOffset;
		}
		else {
			m_Registry->GetEventQueue().Publish<DestroyChildEntity>(parentEnt);
		}
	}
}