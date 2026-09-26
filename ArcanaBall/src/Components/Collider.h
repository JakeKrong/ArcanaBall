#pragma once
#include <cstdint>

enum class ColliderType : uint8_t {
	Block,
	Ball,
	Paddle,
	Border,
	Effects,
	OutZone
};

enum class ColliderShape : uint8_t {
	Rectangle,
	Circle
};

struct Collider {
	ColliderType colType{ ColliderType::Block };		//Default value
	ColliderShape colShape{ ColliderShape::Rectangle };	//Default value
};