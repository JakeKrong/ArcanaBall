#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

struct InputState {
	//Keyboard
	bool activeFire = false; //Example
	bool timeSlow = false;

	//Mouse
	bool mouseClicked = false;
	bool mouseReleased = false;
	sf::Vector2f mousePos;
	bool mouseWithinBounds = true;

	//Game State
	bool pauseGame = false;
	bool resumeGame = false;
};


class InputManager {
public:
	void Update(sf::RenderWindow&);
	void HandleEvent(const sf::Event&);

	InputState& GetInputStates();
	void ResetInputs();

private:
	InputState m_InputState;
};