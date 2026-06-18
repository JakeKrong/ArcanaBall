#include "Game.h"
#include "MainMenuState.h"
#include "PlayingState.h"
#include "GameStateEvent.h"
#include "FileReader.h"

#include <cassert>

//Testing
#include <iostream>

Game::Game() :
	m_Window(sf::RenderWindow(sf::VideoMode(DefaultResolution), "Arcana Ball"))
{
	m_Window.setFramerateLimit(TargetFixedUpdateFreq);
	m_Window.setVerticalSyncEnabled(false);
}

void Game::Run() {
	sf::Clock timer;

	//Load Service Manager assets and Level Data
	if (!FileReader::ReadLevelData(m_LevelDataCache)) return;
	m_AudioManager.PreloadAudio();

	//Set initial state
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

		m_AudioManager.UpdateChannels(m_Registry.GetEventQueue());

		//Consume Game State Events
		for (auto event : m_Registry.GetEventQueue().GetTEvents<GameStateEvent>()) {
			auto stateEvent = event->type;
			float eventPayload = static_cast<int>(event->payload);
			switch(stateEvent) {
			case(GameStateEvent::Type::StartGame):
				m_Registry.ResetManagers();
				m_StateManager.EnqueueChangeState(std::make_unique<PlayingState>(this, eventPayload, GetStageGridData(eventPayload)));
				break;
			case(GameStateEvent::Type::EndGame):
				m_Window.close();
				break;
			case(GameStateEvent::Type::ContinueGame):
				m_InputManager.GetInputStates().resumeGame = true;
				break;
			case(GameStateEvent::Type::MainMenu):
				m_Registry.ResetManagers();
				m_StateManager.EnqueueChangeState(std::make_unique<MainMenuState>(this));
			}
		}

		//Clear input state and event queue
		m_InputManager.ResetInputs();
		m_Registry.GetEventQueue().ClearEvents();

		//####### DEBBUGGING #######//
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) std::cout<<"\n\n\n\n\n\n\n\n\n";
	}
}

Registry& Game::GetRegistry() { return m_Registry; }
InputManager& Game::GetInputManager() { return m_InputManager; }
TextureManager& Game::GetTextureManager() { return m_TextureManager; }
AudioManager& Game::GetAudioManager() { return m_AudioManager; }

StageGridData& Game::GetStageGridData(int level) { 
	assert(m_LevelDataCache.contains(level) && "Loading unknown level data!");
	return m_LevelDataCache.at(level); 
}

void Game::SetMouseVisibility(bool visibility) { m_Window.setMouseCursorVisible(visibility); }