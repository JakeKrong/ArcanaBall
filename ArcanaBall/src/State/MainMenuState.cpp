#include "MainMenuState.h"
#include "Game.h"

#include "RenderSystem.h"
#include "UISystem.h"
#include "Component.h"
#include "Prefabs.h"

#include "MenuNavEvent.h"

MainMenuState::MainMenuState(Game* game) : 
	IState(game),
	m_RenderSystem(game->GetRegistry().RegisterSystem<RenderSystem>()),
	m_UISystem(game->GetRegistry().RegisterSystem<UISystem>())
{
	m_OverlayEnt.reserve(10);
}

void MainMenuState::Enter()
{
	Registry& registry = m_Game->GetRegistry();
	
	// *** Components ****//
	//Register
	registry.RegisterComponent<Transform>();
	registry.RegisterComponent<Renderable>();
	registry.RegisterComponent<RendText>();
	registry.RegisterComponent<Button>();
	registry.RegisterComponent<AnimationData>();

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
	registry.AddComponentToEntity<Button>(startBtn, ButtonAction::StartGame, 1);

	Entity selectStgBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(selectStgBtn, sf::Vector2f{ 490,400 }, sf::Vector2f{ 300,50 });
	registry.AddComponentToEntity<Renderable>(selectStgBtn, &m_Game->GetTextureManager().Load("UI/Select_Level"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(selectStgBtn, ButtonAction::SelectLevel);

	Entity closeBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(closeBtn, sf::Vector2f{ 490,500 }, sf::Vector2f{ 300,50 });
	registry.AddComponentToEntity<Renderable>(closeBtn, &m_Game->GetTextureManager().Load("UI/Quit_Game"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(closeBtn, ButtonAction::ExitGame);

	Entity guideBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(guideBtn, sf::Vector2f{ 850,325 }, sf::Vector2f{ 60,60 });
	registry.AddComponentToEntity<Renderable>(guideBtn, &m_Game->GetTextureManager().Load("UI/Question"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(guideBtn, ButtonAction::OpenGuide);

	Prefab::UI::VolumeControl(registry, m_Game->GetTextureManager(), m_Game->GetAudioManager().getCurVolume());

	m_Game->GetAudioManager().PlayMusic("Rivellon_8bit");
}

void MainMenuState::Exit()
{

}

void MainMenuState::Update(float deltaTime)
{
	auto& playerInput = m_Game->GetInputManager().GetInputStates();

	m_UISystem.Update(playerInput);

	//Process menu navigation event if triggered
	for (auto event : m_Game->GetRegistry().GetEventQueue().GetTEvents<MenuNavEvent>()) {
		switch (event->screen) {
		case MenuNavEvent::NavScreen::MainMenu:
		{
			Registry& reg = m_Game->GetRegistry();
			for (auto ent : m_OverlayEnt) {
				reg.DestroyEntity(ent);
			}
			m_UISystem.SetButtonsEnabled(true);
			break;
		}
		case MenuNavEvent::NavScreen::SelectLevel:
			SelectLevel();
			break;
		case MenuNavEvent::NavScreen::GameGuide:
			GameGuide();
			break;
		}
	}
}

void MainMenuState::Render(sf::RenderWindow& renderWindow)
{
	m_RenderSystem.Update(renderWindow);
}

void MainMenuState::SelectLevel() {
	m_UISystem.SetButtonsEnabled(false);
	
	Registry& registry = m_Game->GetRegistry();

	Entity darkOverlay = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(darkOverlay, Transform{ {0,0}, DefaultResolution });
	registry.AddComponentToEntity<Renderable>(darkOverlay, &m_Game->GetTextureManager().Load("Overlay"), RenderLayer::UI);
	m_OverlayEnt.push_back(darkOverlay);

	Entity level1Button = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(level1Button, sf::Vector2f{ 490,100 }, sf::Vector2f{ 300,75 });
	registry.AddComponentToEntity<Renderable>(level1Button, &m_Game->GetTextureManager().Load("UI/Level_1"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(level1Button, ButtonAction::StartGame, 1);
	m_OverlayEnt.push_back(level1Button);

	Entity level2Button = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(level2Button, sf::Vector2f{ 490,225 }, sf::Vector2f{ 300,75 });
	registry.AddComponentToEntity<Renderable>(level2Button, &m_Game->GetTextureManager().Load("UI/Level_2"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(level2Button, ButtonAction::StartGame, 2);
	m_OverlayEnt.push_back(level2Button);

	Entity level3Button = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(level3Button, sf::Vector2f{ 490,350 }, sf::Vector2f{ 300,75 });
	registry.AddComponentToEntity<Renderable>(level3Button, &m_Game->GetTextureManager().Load("UI/Level_3"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(level3Button, ButtonAction::StartGame, 3);
	m_OverlayEnt.push_back(level3Button);

	Entity level4Button = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(level4Button, sf::Vector2f{ 490,475 }, sf::Vector2f{ 300,75 });
	registry.AddComponentToEntity<Renderable>(level4Button, &m_Game->GetTextureManager().Load("UI/Level_4"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(level4Button, ButtonAction::StartGame, 4);
	m_OverlayEnt.push_back(level4Button);

	Entity backButton = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(backButton, sf::Vector2f{ 590,600 }, sf::Vector2f{ 100,75 });
	registry.AddComponentToEntity<Renderable>(backButton, &m_Game->GetTextureManager().Load("UI/Back"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(backButton, ButtonAction::CloseMenuOverlay);
	m_OverlayEnt.push_back(backButton);
}

void MainMenuState::GameGuide() {
	m_UISystem.SetButtonsEnabled(false);

	Registry& registry = m_Game->GetRegistry();

	Entity darkOverlay = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(darkOverlay, Transform{ {0,0}, DefaultResolution });
	registry.AddComponentToEntity<Renderable>(darkOverlay, &m_Game->GetTextureManager().Load("Overlay"), RenderLayer::UI);
	m_OverlayEnt.push_back(darkOverlay);

	Entity backButton = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(backButton, sf::Vector2f{ 590,600 }, sf::Vector2f{ 100,75 });
	registry.AddComponentToEntity<Renderable>(backButton, &m_Game->GetTextureManager().Load("UI/Back"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(backButton, ButtonAction::CloseMenuOverlay);
	m_OverlayEnt.push_back(backButton);

}