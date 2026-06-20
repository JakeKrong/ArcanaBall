#pragma once
#include "BaseEvent.h"
#include "SFML/System/Vector2.hpp"
#include <variant>

struct SpawnEntityEvent : BaseEvent {
	enum class Type { Enemy, BloodEffect, Etc };
	Type type;

	std::variant<int, float> payload; //Insert data for each class type
};

struct SpawnEffectsEvent : BaseEvent {
	enum class EffectType { WoodBreak };

	SpawnEffectsEvent() = default;
	SpawnEffectsEvent(EffectType type, sf::Vector2f payload) :
		type(type),
		payload(payload)
	{ }

	EffectType type;

	sf::Vector2f payload; //Insert data for each class type
};