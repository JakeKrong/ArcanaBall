#pragma once
#include "StatusEffect.h"
#include <array>

enum class BlockType {
	Stone, //Default
	Wood,
	Brick,
	Steel
};

// Resistances Matrix -> Stores Resistances for Fire, Ice, and Lightning sequentially
// Values: >
// 0 : Vulnerable  - ABLE to take damage of given type, and ABLE to hold the elemental infusion on the block
// 1 : Susceptible - ABLE to take damange of corresponding type, but is UNABLE to hold elemental infusion
// 2 : Immune	   - UNABLE to take damage of incoming type, also unable to hold elemental infusion of given type

struct Block {
	enum ResistanceLvl {
		Vulnerable,
		Susceptible,
		Immune
	};

	using ResistMatrix = std::array<ResistanceLvl, 3>;

	Block() = default;

	Block(BlockType type, float durability, ResistMatrix resist) :
		blockType(type),
		durability(durability),
		maxDurability(durability),
		blockResist(resist)
	{}

	BlockType blockType = BlockType::Stone;
	float durability = 1.f;
	float maxDurability = 1.f;
	ResistMatrix blockResist{};
};