#include "BlockSystem.h"
#include "Registry.h"
#include "Prefabs.h"
#include "AudioAsset.h"

#include "BlockCollisionEvent.h"
#include "EntityDestroyedEvent.h"
#include "SpawnEntityEvent.h"
#include "GameStateEvent.h"
#include "AudioEvent.h"
#include "ChangeTextureEvent.h"

void BlockSystem::Update() {
	auto& blockCompArr = m_Registry->GetComponentArray<Block>();
	auto& statusEffCompArr = m_Registry->GetComponentArray<StatusEffect>();

	//Consume block collision events
	for (auto& event : m_Registry->GetEventQueue().GetTEvents<BlockCollisionEvent>()) {
		const Entity blockEntity = event->blockEntity;
		auto& blockComp = blockCompArr.GetTComponent(blockEntity);
		auto& statusEffComp = statusEffCompArr.GetTComponent(blockEntity);
		auto& transComp = m_Registry->GetEntityComponent<Transform>(blockEntity);

		float currDurability = blockComp.durability;

		if (event->reaction == ActiveReaction::None) { //If non reaction collision (ball collision)
			QueueBlockAudio(blockComp.blockType, false);
		}

		if (event->infusedElem == ElemInfusion::None) { //Regular uninfused ball collision
			blockComp.durability -= 1;
			UpdateBlockSprite(blockComp.blockType, blockEntity, currDurability, blockComp.durability, blockComp.maxDurability);
		}
		else { // If collider of entity with block has an active elemental infusion

			Block::ResistanceLvl resistLvl = blockComp.blockResist[static_cast<int>(event->infusedElem) - 1];
			switch (resistLvl) {
			case(Block::ResistanceLvl::Immune):
				continue;
				break;
			case(Block::ResistanceLvl::Susceptible):
				blockComp.durability -= 1;

				//If have current infusion trigger reaction
				if (!(statusEffComp.element == ElemInfusion::None)) {
					TriggerReaction(statusEffComp.element, event->infusedElem, transComp.position);
					//Clear infused element
					statusEffComp.element = ElemInfusion::None;
					m_Registry->GetEventQueue().Publish<DestroyChildEntity>(event->blockEntity);
				}
				UpdateBlockSprite(blockComp.blockType, blockEntity, currDurability, blockComp.durability, blockComp.maxDurability);

				break;
			case(Block::ResistanceLvl::Vulnerable):
				blockComp.durability -= 1;
				//If no current infusion and durability not yet 0, infuse element
				if (statusEffComp.element == ElemInfusion::None && blockComp.durability > 0) {
					//Infuse and spawn effect
					statusEffComp.element = event->infusedElem;
					m_Registry->GetEventQueue().Publish<SpawnEffectsEvent>({ event->infusedElem, transComp.position, event->blockEntity });
				}
				//If have current infusion and collider's infusion is different from current
				else if (statusEffComp.element != ElemInfusion::None && statusEffComp.element != event->infusedElem) {
					TriggerReaction(statusEffComp.element, event->infusedElem, transComp.position);
					//Clear infused element
					statusEffComp.element = ElemInfusion::None;
					m_Registry->GetEventQueue().Publish<DestroyChildEntity>(event->blockEntity);
				}
				UpdateBlockSprite(blockComp.blockType, blockEntity, currDurability, blockComp.durability, blockComp.maxDurability);
				break;
			}
		}
	}

	auto entities = m_Entities;
	for (Entity ent : entities) {
		auto& blockComp = blockCompArr.GetTComponent(ent);
		if (blockComp.durability <= 0) {
			sf::Vector2f blockPos = m_Registry->GetEntityComponent<Transform>(ent).position;
			EventQueue& eventQ = m_Registry->GetEventQueue();

			eventQ.PublishDeferred<BlockDestroyed>(blockPos); //Event to notify CollisionSystem for update on BlockGridMap

			//Publish spawning block break effect event
			eventQ.Publish<SpawnEffectsEvent>({ blockComp.blockType, blockPos });

			m_Registry->DestroyEntity(ent);
			eventQ.Publish<DestroyChildEntity>(ent); //Destroy any status effect animations

			QueueBlockAudio(blockComp.blockType, true);
		}
	}

	if (!m_Entities.size()) { m_Registry->GetEventQueue().Publish<GameStateEvent>({ GameStateEvent::Type::GameOver, 1 }); }
}

void BlockSystem::TriggerReaction(ElemInfusion elem1, ElemInfusion elem2, sf::Vector2f blockPos) {
	//Trigger Ice Shatter
	if ((elem1 == ElemInfusion::Fire && elem2 == ElemInfusion::Ice) ||
		(elem1 == ElemInfusion::Ice && elem2 == ElemInfusion::Fire)) {
		m_Registry->GetEventQueue().Publish<SpawnEffectsEvent>({ ActiveReaction::IceShatter, blockPos });
	}
	//Trigger Overload
	else if ((elem1 == ElemInfusion::Fire && elem2 == ElemInfusion::Lightning) ||
			 (elem1 == ElemInfusion::Lightning && elem2 == ElemInfusion::Fire)){
		m_Registry->GetEventQueue().Publish<SpawnEffectsEvent>({ ActiveReaction::Overload, blockPos });
	}
	//Trigger Lightning Cross
	else if ((elem1 == ElemInfusion::Ice && elem2 == ElemInfusion::Lightning) ||
		(elem1 == ElemInfusion::Lightning && elem2 == ElemInfusion::Ice)) {
		m_Registry->GetEventQueue().Publish<SpawnEffectsEvent>({ ActiveReaction::LightningCross, blockPos });
	}
}

void BlockSystem::QueueBlockAudio(BlockType type, bool isDestroyed) {

	AudioAsset hitAudio{};

	switch (type) {
	case(BlockType::Stone): 
		hitAudio = AudioAsset::FX_StoneBreak;
		break;
	case(BlockType::Brick):
		hitAudio = !isDestroyed ? AudioAsset::FX_BrickHit : AudioAsset::FX_BrickBreak;
		break;
	case(BlockType::Wood):
		hitAudio = !isDestroyed ? AudioAsset::FX_WoodHit : AudioAsset::FX_WoodBreak;
		break;
	case(BlockType::Steel):
		hitAudio = !isDestroyed ? AudioAsset::FX_SteelHit : AudioAsset::FX_SteelBreak;
		break;
	}
	m_Registry->GetEventQueue().Publish<AudioEvent>(hitAudio);
}

void BlockSystem::UpdateBlockSprite(BlockType type, Entity ent, float prevDurability, float newDurability, float maxDurability) {
	switch (type) {
	case(BlockType::Brick):
		if (prevDurability > maxDurability / 2 && newDurability <= maxDurability / 2)
			m_Registry->GetEventQueue().Publish<ChangeTextureEvent>({ ent, "GameObject/Brick_Damaged" });
		break;
	case(BlockType::Wood):
		if (prevDurability > maxDurability / 2 && newDurability <= maxDurability / 2)
			m_Registry->GetEventQueue().Publish<ChangeTextureEvent>({ ent, "GameObject/Wood_Damaged" });
		break;
	case(BlockType::Steel): // 2 damaged block variants
		if (prevDurability > maxDurability / 3 * 2 && newDurability <= maxDurability / 3 * 2)
			m_Registry->GetEventQueue().Publish<ChangeTextureEvent>({ ent, "GameObject/Steel_Damaged_1" });
		else if (prevDurability > maxDurability / 3 && newDurability <= maxDurability / 3) {
			m_Registry->GetEventQueue().Publish<ChangeTextureEvent>({ ent, "GameObject/Steel_Damaged_2" });
		}
		break;
	}
}