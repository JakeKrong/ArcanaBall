#include "BlockSystem.h"
#include "Registry.h"
#include "Prefabs.h"

#include "Block.h"
#include "BlockCollisionEvent.h"
#include "EntityDestroyedEvent.h"
#include "SpawnEntityEvent.h"
#include "GameStateEvent.h"

void BlockSystem::Update() {
	auto& blockCompArr = m_Registry->GetComponentArray<Block>();
	auto& statusEffCompArr = m_Registry->GetComponentArray<StatusEffect>();

	//Consume block collision events
	for (auto& event : m_Registry->GetEventQueue().GetTEvents<BlockCollisionEvent>()) {
		auto& blockComp = blockCompArr.GetTComponent(event->blockEntity);
		auto& statusEffComp = statusEffCompArr.GetTComponent(event->blockEntity);

		switch (blockComp.blockType) {
			blockComp.durability -= 1;
		case (BlockType::Stone):
			blockComp.durability -= 1;
			break;
		case (BlockType::Wood):
			blockComp.durability -= 1;
			if (blockComp.durability <= 0) {
				m_Registry->GetEventQueue().Publish<SpawnEffectsEvent>({SpawnEffectsEvent::EffectType::WoodBreak,  
					m_Registry->GetEntityComponent<Transform>(event->blockEntity).position});
			}
			break;
		case (BlockType::Brick):
			blockComp.durability -= 1;
			break;
		case (BlockType::Steel):
			blockComp.durability -= 1;
			break;
		default:
			blockComp.durability -= 1;
			break;
		}
	}

	for (Entity ent : m_Entities) {
		auto& blockComp = blockCompArr.GetTComponent(ent);
		if (blockComp.durability <= 0) {
			m_Registry->GetEventQueue().PublishDeferred<BlockDestroyed>({ m_Registry->GetEntityComponent<Transform>(ent).position });
			m_Registry->DestroyEntity(ent);
		}
	}

	if (!m_Entities.size()) { m_Registry->GetEventQueue().Publish<GameStateEvent>({ GameStateEvent::Type::GameOver, 1 }); }
}