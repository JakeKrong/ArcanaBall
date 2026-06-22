#pragma once
#include "BaseEvent.h"
#include "SFML/System/Vector2.hpp"
#include "Block.h"

#include <variant>

struct SpawnEntityEvent : BaseEvent {
	enum class Type { Enemy, BloodEffect, Etc };
	Type type;

	std::variant<int, float> payload; //Insert data for each class type
};

struct SpawnEffectsEvent : BaseEvent {
	using EffectType = std::variant<BlockType, ElemInfusion, ActiveReaction>;

	SpawnEffectsEvent(EffectType type, sf::Vector2f payload, Entity ent = 0) :
		type(type),
		payload(payload),
		parentEntity(ent)
	{ }

	EffectType type;
	sf::Vector2f payload; //Insert data for each class type
	Entity parentEntity{ 0 };
};