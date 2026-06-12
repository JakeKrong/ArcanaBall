#pragma once
#include "IState.h"
#include "Types.h"

#include "UISystem.h"
#include "RenderSystem.h"

class MainMenuState : public IState{
public:
	explicit MainMenuState(Game*);

	void Enter() override;
	void Exit() override;

	void Update(float) override;
	void Render(sf::RenderWindow&) override;

private:
	RenderSystem& m_RenderSystem;
	UISystem& m_UISystem;
};