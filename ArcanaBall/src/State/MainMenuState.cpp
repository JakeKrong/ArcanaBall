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
	registry.AddComponentToEntity<Renderable>(background, &m_Game->GetTextureManager().Load("Forest_Background"), RenderLayer::Background);

	Entity title = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(title, sf::Vector2f{ 440,50 }, sf::Vector2f{ 400, 200});
	registry.AddComponentToEntity<Renderable>(title, &m_Game->GetTextureManager().Load("Title_Art"), RenderLayer::Background);

	Entity startBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(startBtn, sf::Vector2f{ 490,300 }, sf::Vector2f{ 300,50 });
	registry.AddComponentToEntity<Renderable>(startBtn, &m_Game->GetTextureManager().Load("UI/Start"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(startBtn, ButtonAction::StartGame, 1.f);

	Entity selectStgBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(selectStgBtn, sf::Vector2f{ 490,400 }, sf::Vector2f{ 300,50 });
	registry.AddComponentToEntity<Renderable>(selectStgBtn, &m_Game->GetTextureManager().Load("UI/Select_Level"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(selectStgBtn, ButtonAction::StartGame, 1.f);

	Entity closeBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(closeBtn, sf::Vector2f{ 490,500 }, sf::Vector2f{ 300,50 });
	registry.AddComponentToEntity<Renderable>(closeBtn, &m_Game->GetTextureManager().Load("UI/Quit_Game"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(closeBtn, ButtonAction::ExitGame);

	Entity guideBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(guideBtn, sf::Vector2f{ 850,350 }, sf::Vector2f{ 40,50 });
	registry.AddComponentToEntity<Renderable>(guideBtn, &m_Game->GetTextureManager().Load("UI/Question"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(guideBtn, ButtonAction::NoAction);
}

void MainMenuState::Exit()
{

}

void MainMenuState::Update(float deltaTime)
{
	auto& playerInput = m_Game->GetInputManager().GetInputStates();

	m_UISystem.Update(playerInput);
}

void MainMenuState::Render(sf::RenderWindow& renderWindow)
{
	m_RenderSystem.Update(renderWindow);
}