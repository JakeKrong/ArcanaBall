#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

struct InputState {
	//Keyboard
	bool activeFire = false; //Example
	bool timeSlow = false;
	bool pauseGame = false;

	//Mouse
	bool mouseClicked = false;
	bool mouseReleased = false;
	sf::Vector2i mousePos;

	//Window
	bool windowFocused = true;
};


class InputManager {
public:
	void Update(sf::RenderWindow&);
	void HandleEvent(const sf::Event&);

	InputState& GetInputStates();
	void ResetMouseClicked();

private:
	InputState m_InputState;
};