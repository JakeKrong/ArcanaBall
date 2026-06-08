#pragma once
#include <variant>
#include <cstdint>

enum class ActionType : uint8_t {
	//State Control
	StartGame,
	ExitGame,
	RestartGame,
	ResumeGame
};

using ButtonPayload = std::variant<int, float>;

class Button {
	
	Button(ActionType action, ButtonPayload payload) :
		buttonAction(action),
		buttonPayload(payload)
	{}

	ButtonPayload buttonPayload;
	ActionType buttonAction;
	bool isEnabled{ true };
	bool isHeld{ false };
};