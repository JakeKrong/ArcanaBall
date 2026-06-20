#include "InputManager.h"

void InputManager::Update(sf::RenderWindow& window) {
	if (m_InputState.mouseWithinBounds) m_InputState.mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) m_InputState.pauseGame = true;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) m_InputState.mouseHeld = true;
}

void InputManager::HandleEvent(const sf::Event& event) {
	if (event.is<sf::Event::MouseButtonPressed>()) {
		if (event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
			m_InputState.mouseClicked = true;
		}
	}
	else if (event.is<sf::Event::MouseButtonReleased>()) {
		if (event.getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) {
			m_InputState.mouseReleased = true;
		}
	}
	else if (event.is<sf::Event::KeyPressed>()) {
		if (event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space)
		m_InputState.activeTimeSlow = true;
	}
	else if (event.getIf<sf::Event::FocusLost>()) {
		m_InputState.pauseGame = true;
	}
	else if (event.getIf<sf::Event::MouseLeft>()) {
		m_InputState.mouseWithinBounds = false;
	}
	else if (event.getIf<sf::Event::MouseEntered>()) {
		m_InputState.mouseWithinBounds = true;
	}
	else if (event.getIf<sf::Event::Resized>()) {
		m_InputState.pauseGame = true;
	}
}

InputState& InputManager::GetInputStates() {
	return m_InputState;
}

void InputManager::ResetInputs() {
	m_InputState.mouseClicked = false;
	m_InputState.mouseReleased = false;
	m_InputState.pauseGame = false;

	m_InputState.activeTimeSlow = false;
}