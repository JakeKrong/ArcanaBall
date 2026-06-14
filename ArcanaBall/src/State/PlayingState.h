#pragma once
#include "IState.h"
#include "Systems.h"

class PlayingState : public IState {
public:
	explicit PlayingState(Game*);

	void Enter() override;
	void Exit() override;

	void Update(float) override;
	void Render(sf::RenderWindow&) override;

private:
	RenderSystem& m_RenderSystem;
	UISystem& m_UISystem;
	CollisionSystem& m_CollisionSystem;
	PhysicsSystem& m_PhysicsSystem;
};