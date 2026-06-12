#pragma once
#include "IState.h"

class PlayingState : public IState {
public:
	explicit PlayingState(Game*);

	void Enter() override;
	void Exit() override;

	void Update(float) override;
	void Render(sf::RenderWindow&) override;

};