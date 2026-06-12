#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

#include "Types.h"
#include "IState.h"

class StateManager {
public:
	void Update(float);
	void Render(sf::RenderWindow&);
	void ChangeState(Scope<IState>);
	void EnqueueChangeState(Scope<IState>);

private:
	Scope<IState> m_CurrentState;
	Scope<IState> m_PendingState = nullptr;
};