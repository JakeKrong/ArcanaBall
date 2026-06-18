#pragma once
#include "BaseEvent.h"

struct GameStateEvent : BaseEvent {
	enum class Type { StartGame, EndGame, ContinueGame, MainMenu, GameOver };

	GameStateEvent(Type type, float payload = 0) :
		type(type),
		payload(payload)
	{}

	Type type;
	float payload{};
};