#pragma once
#include "BaseEvent.h"

struct GameStateEvent : public BaseEvent {
	enum class Type { StartGame, EndGame };

	GameStateEvent(Type type):
		type(type)
	{}

	Type type;
};