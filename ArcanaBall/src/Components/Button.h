#pragma once
#include <variant>
#include <cstdint>

enum class ButtonAction : uint8_t {
	NoAction,
	//State Control
	StartGame,
	ExitGame,
	RestartGame,
	ResumeGame,
	MainMenu
};

struct Button {

	Button() = default;

	Button(ButtonAction action, float payload = 0) :
		buttonAction(action),
		buttonPayload(payload)
	{}

	float buttonPayload{ 0 };
	ButtonAction buttonAction{ ButtonAction::NoAction };
	bool isOnPress{ true }; //On press or On release
	bool isEnabled{ true };
};