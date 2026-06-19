#include "HierarchySystem.h"
#include "Child.h"
#include "Registry.h"
#include "EntityDestroyedEvent.h"

void HierarchySystem::Update() {
	auto& childCompArr = m_Registry->GetComponentArray<Child>();
	auto& transCompArr = m_Registry->GetComponentArray<Transform>();

	for (auto& event : m_Registry->GetEventQueue().GetTEvents<DestroyChildEntity>()) {
		for (Entity ent : m_Entities) {
			if (childCompArr.GetTComponent(ent).parentEntity == event->parentEntity) m_Registry->DestroyEntity(ent);
		}
	}

	for (Entity ent : m_Entities) {
		auto& childTrans = transCompArr.GetTComponent(ent);
		auto& parentTrans = transCompArr.GetTComponent(childCompArr.GetTComponent(ent).parentEntity);

		childTrans.position = parentTrans.position + childCompArr.GetTComponent(ent).localOffset;
	}
}