#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Game; //Forward declaration

class IState {
public:
	inline IState(Game* game) : m_Game(game) {};

	virtual ~IState() = default;

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void Update(float) = 0;
	virtual void HandleEvents(const sf::Event&) = 0;
	virtual void Render(sf::RenderWindow&) = 0;

public:
	Game* m_Game;//Consider GameContext (struct of references to managers/window)
};