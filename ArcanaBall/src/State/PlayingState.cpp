#include "PlayingState.h"
#include "Game.h"
#include "Prefabs.h"

#include "GameStateEvent.h"
#include "AudioEvent.h"

PlayingState::PlayingState(Game* game, int levelNumber, StageGridData& stageData) :
	IState(game),
	m_RenderSystem(game->GetRegistry().RegisterSystem<RenderSystem>()),
	m_UISystem(game->GetRegistry().RegisterSystem<UISystem>()),
	m_CollisionSystem(game->GetRegistry().RegisterSystem<CollisionSystem>()),
	m_PhysicsSystem(game->GetRegistry().RegisterSystem<PhysicsSystem>()),
	m_BlockSystem(game->GetRegistry().RegisterSystem<BlockSystem>()),
	m_LevelData( {stageData, levelNumber  })
{
	m_LevelData.overlayEnt.reserve(10);
}

void PlayingState::Enter() {
	Registry& registry = m_Game->GetRegistry();

	// *** Components ****//
	//Register
	registry.RegisterComponent<Transform>();
	registry.RegisterComponent<Renderable>();
	registry.RegisterComponent<Button>();
	registry.RegisterComponent<Collider>();
	registry.RegisterComponent<Physics>();
	registry.RegisterComponent<Block>();
	registry.RegisterComponent<StatusEffect>();

	// *** Systems ****//
	//Set Registry
	m_RenderSystem.SetRegistry(&m_Game->GetRegistry());
	m_UISystem.SetRegistry(&m_Game->GetRegistry());
	m_CollisionSystem.SetRegistry(&m_Game->GetRegistry());
	m_PhysicsSystem.SetRegistry(&m_Game->GetRegistry());
	m_BlockSystem.SetRegistry(&m_Game->GetRegistry());


	//Set Signature
	Signature renderSig;
	renderSig.set(registry.GetComponentID<Transform>());
	renderSig.set(registry.GetComponentID<Renderable>());
	registry.SetSystemSignature<RenderSystem>(renderSig);

	Signature uiSig;
	uiSig.set(registry.GetComponentID<Transform>());
	uiSig.set(registry.GetComponentID<Button>());
	registry.SetSystemSignature<UISystem>(uiSig);

	Signature colliderSig;
	colliderSig.set(registry.GetComponentID<Transform>());
	colliderSig.set(registry.GetComponentID<Collider>());
	registry.SetSystemSignature<CollisionSystem>(colliderSig);

	Signature physSig;
	physSig.set(registry.GetComponentID<Transform>());
	physSig.set(registry.GetComponentID<Physics>());
	registry.SetSystemSignature<PhysicsSystem>(physSig);

	Signature blockSig;
	blockSig.set(registry.GetComponentID<Block>());
	registry.SetSystemSignature<BlockSystem>(blockSig);

	//Set up level
	GenerateLevelBlocks();
	Prefab::GameObject::LevelBorders(registry, m_Game->GetTextureManager());
	Prefab::GameObject::KillZone(registry);
	Prefab::GameObject::Ball(registry, m_Game->GetTextureManager());

	//Set Paddle
	m_LevelData.paddleEnt = Prefab::GameObject::Paddle(registry, m_Game->GetTextureManager());


	Entity background = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(background, sf::Vector2f{ 0,0 }, sf::Vector2f(DefaultResolution));
	registry.AddComponentToEntity<Renderable>(background, &m_Game->GetTextureManager().Load("Forest_Background"), RenderLayer::Background);
	Entity overlay = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(overlay, sf::Vector2f{ 270,0 }, sf::Vector2f({ 720,720 }));
	registry.AddComponentToEntity<Renderable>(overlay, &m_Game->GetTextureManager().Load("Overlay"), RenderLayer::Background);

	m_CollisionSystem.RegisterCollisionHandlers();
	m_CollisionSystem.InitBlockGridMap();
	m_Game->SetMouseVisibility(false);
}

void PlayingState::Exit() {

}

void PlayingState::Update(float deltaTime) {
	auto& playerInput = m_Game->GetInputManager().GetInputStates();
	auto& registry = m_Game->GetRegistry();

	if (m_LevelData.gameOverEnqueued) {
		if (m_LevelData.gameOver) {
			m_UISystem.Update(playerInput);
			return;
		}
		else if (m_LevelData.gameOverTimer > 0) {
			m_LevelData.gameOverTimer -= deltaTime;
			deltaTime *= (m_LevelData.gameOverTimer / 5);
		}
		else {
			m_LevelData.gameOver = true;
			GameOver();
		}
	}

	if (!m_LevelData.gamePaused) {
		if (playerInput.pauseGame && !m_LevelData.gameOverEnqueued) {
			playerInput.resumeGame = false;
			PauseGame();
			return;
		}

		UpdatePaddle(playerInput);
		m_CollisionSystem.Update();
		m_PhysicsSystem.Update(deltaTime);
		m_BlockSystem.Update();

		for (auto& event : registry.GetEventQueue().GetTEvents<GameStateEvent>()) {
			if (event->type == GameStateEvent::Type::GameOver && !m_LevelData.gameOverEnqueued) {
				m_LevelData.gameOverEnqueued = true;
				m_LevelData.gameOverTimer = 3.f;
				registry.GetEventQueue().Publish<AudioEvent>({AudioAsset::FX_Slowmo});
			}
		}
	}
	else {
		if (playerInput.resumeGame && !m_LevelData.gameOver) {
			ResumeGame();
			return;
		}
		m_UISystem.Update(playerInput);
	}
}

void PlayingState::Render(sf::RenderWindow& renderWindow) {
	m_RenderSystem.Update(renderWindow);
}

void PlayingState::GenerateLevelBlocks() {
	Registry& registry = m_Game->GetRegistry();
	TextureManager& textureMn= m_Game->GetTextureManager();

	float yPosition = GRID_OFFSET_Y, xPosition = GRID_OFFSET_X;

	for (auto rows : m_LevelData.stageGrid) {
		for (auto col : rows){
			switch (col) {
			case 1:
				Prefab::GameObject::Stone(registry, textureMn, sf::Vector2f{xPosition, yPosition});
				break;
			case 2:
				Prefab::GameObject::Brick(registry, textureMn, sf::Vector2f{ xPosition, yPosition });
				break;
			case 3:
				Prefab::GameObject::Wood(registry, textureMn, sf::Vector2f{ xPosition, yPosition });
				break;
			case 4:
				Prefab::GameObject::Steel(registry, textureMn, sf::Vector2f{ xPosition, yPosition });
				break;
			default:
				break;
			}
			xPosition += BLOCK_WIDTH;
		}
		//Reset X position, move Y postion down
		xPosition = GRID_OFFSET_X;
		yPosition += BLOCK_HEIGHT;
	}
}

void PlayingState::UpdatePaddle(InputState& inputState) {
	Transform& paddleTrans = m_Game->GetRegistry().GetComponentArray<Transform>().GetTComponent(m_LevelData.paddleEnt);
	sf::Vector2f mousePos = inputState.mousePos;

	//Clamp paddle position within world border
	paddleTrans.position.x = std::min(980.f - paddleTrans.size.x ,std::max(275.f, mousePos.x));
}

void PlayingState::PauseGame() {
	m_LevelData.gamePaused = true;

	Registry& registry = m_Game->GetRegistry();

	Entity darkOverlay = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(darkOverlay, Transform{ {0,0}, DefaultResolution });
	registry.AddComponentToEntity<Renderable>(darkOverlay, &m_Game->GetTextureManager().Load("Overlay"), RenderLayer::UI);
	m_LevelData.overlayEnt.push_back(darkOverlay);

	Entity pauseTitle = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(pauseTitle, sf::Vector2f{ 340,150 }, sf::Vector2f{ 600,50 });
	registry.AddComponentToEntity<Renderable>(pauseTitle, &m_Game->GetTextureManager().Load("Game_Paused"), RenderLayer::UI);
	m_LevelData.overlayEnt.push_back(pauseTitle);

	Entity continueBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(continueBtn, sf::Vector2f{ 440,300 }, sf::Vector2f{ 400,50 });
	registry.AddComponentToEntity<Renderable>(continueBtn, &m_Game->GetTextureManager().Load("UI/Continue"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(continueBtn, ButtonAction::ResumeGame);
	m_LevelData.overlayEnt.push_back(continueBtn);

	Entity restartBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(restartBtn, sf::Vector2f{ 390,400 }, sf::Vector2f{ 500,50 });
	registry.AddComponentToEntity<Renderable>(restartBtn, &m_Game->GetTextureManager().Load("UI/Restart"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(restartBtn, ButtonAction::RestartGame, 1.f);
	m_LevelData.overlayEnt.push_back(restartBtn);

	Entity backToMenuBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(backToMenuBtn, sf::Vector2f{ 390,500 }, sf::Vector2f{ 500,50 });
	registry.AddComponentToEntity<Renderable>(backToMenuBtn, &m_Game->GetTextureManager().Load("UI/Back_To_Main"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(backToMenuBtn, ButtonAction::MainMenu);
	m_LevelData.overlayEnt.push_back(backToMenuBtn);

	m_Game->SetMouseVisibility(true);
}

void PlayingState::ResumeGame() {
	m_LevelData.gamePaused = false;
	Registry& registry = m_Game->GetRegistry();

	for (Entity ent : m_LevelData.overlayEnt) {
		registry.DestroyEntity(ent);
	}
	m_LevelData.overlayEnt.clear();
	m_Game->SetMouseVisibility(false);
}

void PlayingState::GameOver() {
	Registry& registry = m_Game->GetRegistry();

	Entity darkOverlay = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(darkOverlay, Transform{ {0,0}, DefaultResolution });
	registry.AddComponentToEntity<Renderable>(darkOverlay, &m_Game->GetTextureManager().Load("Overlay"), RenderLayer::UI);

	Entity gameOverTitle = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(gameOverTitle, sf::Vector2f{ 340,150 }, sf::Vector2f{ 600,50 });
	registry.AddComponentToEntity<Renderable>(gameOverTitle, &m_Game->GetTextureManager().Load("Game_Over"), RenderLayer::UI);

	Entity restartLvlBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(restartLvlBtn, sf::Vector2f{ 390,300 }, sf::Vector2f{ 500,50 });
	registry.AddComponentToEntity<Renderable>(restartLvlBtn, &m_Game->GetTextureManager().Load("UI/Restart"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(restartLvlBtn, ButtonAction::StartGame, static_cast<float>(m_LevelData.levelNumber));

	Entity backToMenuBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(backToMenuBtn, sf::Vector2f{ 390,400 }, sf::Vector2f{ 500,50 });
	registry.AddComponentToEntity<Renderable>(backToMenuBtn, &m_Game->GetTextureManager().Load("UI/Back_To_Main"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(backToMenuBtn, ButtonAction::MainMenu);

	m_Game->SetMouseVisibility(true);
}