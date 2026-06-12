#include "MainMenuState.h"
#include "Game.h"

#include "RenderSystem.h"
#include "UISystem.h"
#include "Component.h"
#include <iostream>

MainMenuState::MainMenuState(Game* game) : 
	IState(game),
	m_RenderSystem(game->GetRegistry().RegisterSystem<RenderSystem>()),
	m_UISystem(game->GetRegistry().RegisterSystem<UISystem>())
{}

void MainMenuState::Enter()
{
	Registry& registry = m_Game->GetRegistry();
	
	// *** Components ****//
	//Register
	registry.RegisterComponent<Transform>();
	registry.RegisterComponent<Renderable>();
	registry.RegisterComponent<Button>();

	// *** Systems ****//
	//Set Registry
	m_RenderSystem.SetRegistry(&m_Game->GetRegistry());
	m_UISystem.SetRegistry(&m_Game->GetRegistry());

	//Set Signature
	Signature renderSig;
	renderSig.set(registry.GetComponentID<Transform>());
	renderSig.set(registry.GetComponentID<Renderable>());
	registry.SetSystemSignature<RenderSystem>(renderSig);

	Signature uiSig;
	uiSig.set(registry.GetComponentID<Transform>());
	uiSig.set(registry.GetComponentID<Button>());
	registry.SetSystemSignature<UISystem>(uiSig);

	Entity background = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(background, sf::Vector2f{ 0,0 }, sf::Vector2f(DefaultResolution));
	registry.AddComponentToEntity<Renderable>(background, &m_Game->GetTextureManager().Load("Placeholder_Background"), RenderLayer::Background);

	Entity startBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(startBtn, sf::Vector2f{ 400,500 }, sf::Vector2f{ 200,100 });
	registry.AddComponentToEntity<Renderable>(startBtn, &m_Game->GetTextureManager().Load("Button_StartGame_Transparent"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(startBtn, ButtonAction::StartGame);

	Entity closeBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(closeBtn, sf::Vector2f{ 700,500 }, sf::Vector2f{ 200,100 });
	registry.AddComponentToEntity<Renderable>(closeBtn, &m_Game->GetTextureManager().Load("Button_QuitGame"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(closeBtn, ButtonAction::ExitGame);
}

void MainMenuState::Exit()
{

}

void MainMenuState::Update(float deltaTime)
{
	auto& playerInput = m_Game->GetInputManager().GetInputStates();

	m_UISystem.Update(playerInput);
}

void MainMenuState::Render(sf::RenderWindow& window)
{
	m_RenderSystem.Update(window);
}