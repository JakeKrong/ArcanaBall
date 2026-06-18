#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "Registry.h"
#include "StateManager.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "AudioManager.h"

class Game {
public:
	Game();
	~Game() = default;

	void Run();

	//Manager Getters
	Registry& GetRegistry();
	InputManager& GetInputManager();
	TextureManager& GetTextureManager();
	AudioManager& GetAudioManager();

	StageGridData& GetStageGridData(int);
	void SetMouseVisibility(bool);

private:
	Registry m_Registry;
	sf::RenderWindow m_Window;

	StateManager m_StateManager;
	InputManager m_InputManager;
	TextureManager m_TextureManager;
	AudioManager m_AudioManager;

	std::unordered_map<int, StageGridData> m_LevelDataCache;
};