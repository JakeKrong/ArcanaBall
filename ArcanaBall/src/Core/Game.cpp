#include "Game.h"
#include "MainMenuState.h"
#include "GameStateEvent.h"

Game::Game() :
	m_Window(sf::RenderWindow(sf::VideoMode(DefaultResolution), "Arcana Ball"))
{
	m_Window.setFramerateLimit(TargetFixedUpdateFreq);
	m_Window.setVerticalSyncEnabled(false);
}

void Game::Run() {
	sf::Clock timer;

	m_StateManager.ChangeState(std::make_unique<MainMenuState>(this));

	while (m_Window.isOpen()) {
		float deltaTime = timer.restart().asSeconds();
		if (deltaTime > 0.1) deltaTime = 0.1; //Clamp deltaTime to max

		//Poll and process game window events
		while (auto event = m_Window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) m_Window.close();

			m_InputManager.HandleEvent(*event);
		}
		m_InputManager.Update(m_Window);

		//Perform Game Update and Render
		m_StateManager.Update(deltaTime);
		m_StateManager.Render(m_Window);

		//Consume Game State Events
		for (auto event : m_Registry.GetEventQueue().GetTEvents<GameStateEvent>()) {
			if (event->type == GameStateEvent::Type::StartGame) {
				m_Registry.ResetManagers();
				m_StateManager.EnqueueChangeState(std::make_unique<MainMenuState>(this));
				break;
			}
			else if (event->type == GameStateEvent::Type::EndGame) {
				m_Window.close();
			}
		}

		//Clear input state and event queue
		m_InputManager.ResetMouseClicked();
		m_Registry.GetEventQueue().ClearEvents();
	}
}

Registry& Game::GetRegistry() { return m_Registry; }
InputManager& Game::GetInputManager() { return m_InputManager; }
TextureManager& Game::GetTextureManager() { return m_TextureManager; }
AudioManager& Game::GetAudioManager() { return m_AudioManager; }