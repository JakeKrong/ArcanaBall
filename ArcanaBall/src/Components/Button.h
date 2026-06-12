#pragma once
#include <variant>
#include <cstdint>

enum class ButtonAction : uint8_t {
	NoAction,
	//State Control
	StartGame,
	ExitGame,
	RestartGame,
	ResumeGame
};

using ButtonPayload = std::variant<uint8_t>; //Adjust to actual payload later

struct Button {

	Button() = default;

	Button(ButtonAction action) :
		buttonAction(action)
	{}

	ButtonPayload buttonPayload{ uint8_t{ 0 } };
	ButtonAction buttonAction{ ButtonAction::NoAction };
	bool isOnPress{ true }; //On press or On release
	bool isEnabled{ true };
};