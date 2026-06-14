#pragma once
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

	ColliderType colType;
	ColliderShape colShape;
};