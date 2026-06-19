#pragma once
#include <variant>
#include <cstdint>

enum class ButtonAction : uint8_t {
	NoAction,
	//Game State Control
	StartGame,
	ExitGame,
	RestartGame,
	ResumeGame,
	//Main Menu Navigation
	MainMenu,
	CloseMenuOverlay,
	SelectLevel,
	OpenGuide,
	//Volume Control
	AdjustVolume
};

struct Button {

	Button() = default;

	Button(ButtonAction action, int payload = 0) :
		buttonAction(action),
		buttonPayload(payload)
	{}

	int buttonPayload{ 0 };
	ButtonAction buttonAction{ ButtonAction::NoAction };
	bool isEnabled{ true };
	bool checkHold{ false }; //Perform check/action on mouse hold
};