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
	bool windowFocused = true;
	sf::Vector2i mousePos;
};


class InputManager {
public:
	void Update(sf::RenderWindow&);
	const InputState& GetInputStates() const;
	void SetWindowFocus(bool);

private:
	InputState m_InputState;
};