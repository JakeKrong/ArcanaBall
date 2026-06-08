#include "Game.h"

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

			//StateManager -> handle event
		}
		//StateManager -> Update Loop
		//StateManager -> Render Loop
	}
}