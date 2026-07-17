#include "StateManager.h"

void StateManager::Update(float deltaTime) {
	if (!m_PendingState) {
		m_CurrentState->Update(deltaTime);
	}
	else {
		ChangeState(std::move(m_PendingState));
	}
}

void StateManager::Render(sf::RenderWindow& window) {
	m_CurrentState->Render(window);
}

void StateManager::ChangeState(Scope<IState> iState) {
	m_CurrentState = std::move(iState);
	m_CurrentState->Enter();
}

void StateManager::EnqueueChangeState(Scope<IState> iState) {
	if (m_CurrentState) {
		m_CurrentState->Exit();
		m_CurrentState.reset();
	}
	m_PendingState = std::move(iState);
}