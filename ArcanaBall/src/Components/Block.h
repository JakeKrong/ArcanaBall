#pragma once
#include "StatusEffect.h"
#include <bitset>

enum class BlockType {
	Stone, //Default
	Wood,
	Brick,
	Steel
};

struct Block {
	using Resistances = std::bitset<3>;

	BlockType blockType = BlockType::Stone;
	float durability = 1.f;
	Resistances blockResist{};
};