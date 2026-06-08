#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "Registry.h"

class Game {
public:
	Game();
	~Game() = default;

	void Run();

	//Getters for World, Windows, Asset Managers

private:
	Registry m_Registry;
	sf::RenderWindow m_Window;

	//World, Windows, Asset Managers
};