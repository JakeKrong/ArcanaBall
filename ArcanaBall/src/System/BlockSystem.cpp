#include "BlockSystem.h"
#include "Registry.h"

#include "Block.h"
#include "BlockCollisionEvent.h"
#include "EntityDestroyedEvent.h"
#include "GameStateEvent.h"

void BlockSystem::Update() {
	auto& blockCompArr = m_Registry->GetComponentArray<Block>();
	auto& statusEffCompArr = m_Registry->GetComponentArray<StatusEffect>();

	//Consume block collision events
	for (auto& event : m_Registry->GetEventQueue().GetTEvents<BlockCollisionEvent>()) {
		auto& blockComp = blockCompArr.GetTComponent(event->blockEntity);
		auto& statusEffComp = statusEffCompArr.GetTComponent(event->blockEntity);

		switch (blockComp.blockType) {
		case (BlockType::Stone):
			blockComp.durabilty -= 1;
			break;
		case (BlockType::Wood):
			blockComp.durabilty -= 1;
			break;
		case (BlockType::Brick):
			blockComp.durabilty -= 1;
			break;
		case (BlockType::Steel):
			blockComp.durabilty -= 1;
			break;
		default:
			blockComp.durabilty -= 1;
			break;
		}
	}

	for (Entity ent : m_Entities) {
		auto& blockComp = blockCompArr.GetTComponent(ent);
		if (blockComp.durabilty <= 0) {
			m_Registry->GetEventQueue().PublishDeferred<BlockDestroyed>({ m_Registry->GetEntityComponent<Transform>(ent).position });
			m_Registry->DestroyEntity(ent);

		}
	}

	if (!m_Entities.size()) { m_Registry->GetEventQueue().Publish<GameStateEvent>({ GameStateEvent::Type::GameOver, 1.f }); }
}