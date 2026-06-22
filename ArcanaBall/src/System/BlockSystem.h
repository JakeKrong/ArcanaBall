#pragma once
#include "ISystem.h"
#include "Block.h"
#include <SFML/System/Vector2.hpp>

class BlockSystem : public ISystem {
public:
	void Update();

	void TriggerReaction(ElemInfusion, ElemInfusion, sf::Vector2f);
	void UpdateBlockSprite(BlockType, Entity, float, float, float);
	void QueueBlockAudio(BlockType, bool);
};