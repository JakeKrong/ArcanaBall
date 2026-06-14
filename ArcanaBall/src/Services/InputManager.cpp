#include "InputManager.h"

void InputManager::Update(sf::RenderWindow& window) {
	m_InputState.mousePos = sf::Mouse::getPosition(window); //Set clamp later
}

void InputManager::HandleEvent(const sf::Event& event) {
	if (event.is<sf::Event::MouseButtonPressed>()) {
		if (event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
			GetInputStates().mouseClicked = true;
		}
	}
	else if (event.is<sf::Event::MouseButtonReleased>()) {
		if (event.getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) {
			GetInputStates().mouseReleased = true;
		}
	}
	if (event.getIf<sf::Event::FocusGained>()) {
		GetInputStates().windowFocused = true;
	}
	else if (event.getIf<sf::Event::FocusGained>()) {
		GetInputStates().windowFocused = false;
	}
}

InputState& InputManager::GetInputStates() {
	return m_InputState;
}

void InputManager::ResetMouseClicked() {
	m_InputState.mouseClicked = false;
	m_InputState.mouseReleased = false;
}