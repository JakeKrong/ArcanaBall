#pragma once
#include "BaseEvent.h"

struct GameStateEvent : BaseEvent {
	enum class Type { StartGame, EndGame, ContinueGame, MainMenu, GameOver };

	GameStateEvent(Type type, int payload = 0) :
		type(type),
		payload(payload)
	{}

	Type type;
	int payload{};
};