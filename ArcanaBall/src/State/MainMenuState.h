#pragma once
#include "IState.h"

class MainMenuState : public IState{
public:
	explicit MainMenuState(Game*);

	void Enter() override;
	void Exit() override;

	void Update(float);
	void HandleEvents(const sf::Event&);
	void Render(sf::RenderWindow&);
};