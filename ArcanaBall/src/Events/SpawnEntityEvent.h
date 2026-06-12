#pragma once
#include "BaseEvent.h"
#include <variant>

struct SpawnEntityEvent : public BaseEvent {
	enum class Type { Enemy, BloodEffect, Etc };
	Type type;

	std::variant<int, float> payload; //Insert data for each class type
};