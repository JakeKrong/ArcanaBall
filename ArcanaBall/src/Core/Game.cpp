#include "Game.h"
#include "Types.h"

//For testing
#include <iostream>

Game::Game() :
	m_Window(sf::RenderWindow(sf::VideoMode(DefaultResolution), "Arcana Ball"))
{
	m_Window.setFramerateLimit(TargetFixedUpdateFreq);
	m_Window.setVerticalSyncEnabled(false);
}

void Game::Run() {
	sf::Clock timer;

	//StateManager set initial state

	while (m_Window.isOpen()) {
		float deltaTime = timer.restart().asSeconds();
		if (deltaTime > 0.1) deltaTime = 0.1; //Clamp deltaTime to max

		while (auto event = m_Window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) m_Window.close();
			if (event->is<sf::Event::MouseButtonPressed>()) {
				auto mouseClickPos = event.value().getIf<sf::Event::MouseButtonPressed>()->position;
				std::cout << "x:" << mouseClickPos.x << " y:" << mouseClickPos.y << std::endl;
			}
			if (event->is<sf::Event::KeyPressed>()) {
				auto keyPressed = event.value().getIf<sf::Event::KeyPressed>()->code;
				if (keyPressed == sf::Keyboard::Key::A) {
					std::cout << "Key pressed: " << 'A' << std::endl;
				}
				else std::cout << "Key pressed: " << "NOT A" << std::endl;
			}
			if (event->is<sf::Event::KeyReleased>()) {
				auto keyPressed = event.value().getIf<sf::Event::KeyReleased>()->code;
				if (keyPressed == sf::Keyboard::Key::A) {
					std::cout << "Key released: " << 'A' << std::endl;
				}
				else std::cout << "Key released: " << "NOT A" << std::endl;
			}
				

			//StateManager -> handle event
		}
		//StateManager -> Update Loop
		//StateManager -> Render Loop
	}
}