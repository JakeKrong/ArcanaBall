#include "InputManager.h"

void InputManager::Update(sf::RenderWindow& window) {
	if (m_InputState.mouseWithinBounds) m_InputState.mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) m_InputState.pauseGame = true;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) m_InputState.mouseHeld = true;
}

void InputManager::HandleEvent(const sf::Event& event) {
	if (auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouseEvent->button == sf::Mouse::Button::Left) m_InputState.mouseClicked = true;
	}
	else if (auto mouseEvent = event.getIf<sf::Event::MouseButtonReleased>()) {
		if (mouseEvent->button == sf::Mouse::Button::Left) {
			m_InputState.mouseReleased = true;
		}
	}
	else if (auto keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		switch (keyPressed->code) {
		case(sf::Keyboard::Key::Q):
			m_InputState.activeFire = true;
			break;
		case(sf::Keyboard::Key::W):
			m_InputState.activeIce = true;
			break;
		case(sf::Keyboard::Key::E):
			m_InputState.activeLight = true;
			break;
		}
	}
	else if (event.is<sf::Event::FocusLost>()) {
		m_InputState.pauseGame = true;
	}
	else if (event.is<sf::Event::MouseLeft>()) {
		m_InputState.mouseWithinBounds = false;
	}
	else if (event.is<sf::Event::MouseEntered>()) {
		m_InputState.mouseWithinBounds = true;
	}
	else if (event.is<sf::Event::Resized>()) {
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

	m_InputState.activeFire = false;
	m_InputState.activeIce = false;
	m_InputState.activeLight = false;
}