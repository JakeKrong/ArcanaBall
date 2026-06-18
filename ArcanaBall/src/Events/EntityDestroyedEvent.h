#pragma once
#include "BaseEvent.h"

struct BlockDestroyed : BaseEvent {
	BlockDestroyed(sf::Vector2f blockPos) :
		blockPos(blockPos)
	{}

	sf::Vector2f blockPos{ 0, 0 };
};

struct DestroyChildEntity {
	Entity parentEntity{ 0 };
};