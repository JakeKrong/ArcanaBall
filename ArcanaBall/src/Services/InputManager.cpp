#include "InputManager.h"

#include <iostream>

void InputManager::Update(sf::RenderWindow& window) {
	m_InputState.mouseClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	m_InputState.mousePos = sf::Mouse::getPosition(window);

	m_InputState.windowFocused = window.hasFocus();
}

const InputState& InputManager::GetInputStates() const{
	return m_InputState;
}

void InputManager::SetWindowFocus(bool focus) {
	m_InputState.windowFocused = false;
}