#include "PlayingState.h"
#include "Game.h"
#include "Prefabs.h"

#include "GameStateEvent.h"
#include "AudioEvent.h"

//TESTING
#include <iostream>

PlayingState::PlayingState(Game* game, int levelNumber, StageGridData& stageData) :
	IState(game),
	m_RenderSystem(game->GetRegistry().RegisterSystem<RenderSystem>()),
	m_UISystem(game->GetRegistry().RegisterSystem<UISystem>()),
	m_CollisionSystem(game->GetRegistry().RegisterSystem<CollisionSystem>()),
	m_PhysicsSystem(game->GetRegistry().RegisterSystem<PhysicsSystem>()),
	m_BlockSystem(game->GetRegistry().RegisterSystem<BlockSystem>()),
	m_HierarchySystem(game->GetRegistry().RegisterSystem<HierarchySystem>()),
	m_LevelData( {stageData, levelNumber  })
{
	m_OverlayEnt.reserve(10);
}

void PlayingState::Enter() {
	Registry& registry = m_Game->GetRegistry();

	// *** Components ****//
	//Register
	registry.RegisterComponent<Transform>();
	registry.RegisterComponent<Renderable>();
	registry.RegisterComponent<RendText>();
	registry.RegisterComponent<Button>();
	registry.RegisterComponent<Collider>();
	registry.RegisterComponent<Physics>();
	registry.RegisterComponent<Block>();
	registry.RegisterComponent<StatusEffect>();
	registry.RegisterComponent<Child>();

	// *** Systems ****//
	//Set Registry
	m_RenderSystem.SetRegistry(&m_Game->GetRegistry());
	m_UISystem.SetRegistry(&m_Game->GetRegistry());
	m_CollisionSystem.SetRegistry(&m_Game->GetRegistry());
	m_PhysicsSystem.SetRegistry(&m_Game->GetRegistry());
	m_BlockSystem.SetRegistry(&m_Game->GetRegistry());
	m_HierarchySystem.SetRegistry(&m_Game->GetRegistry());


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

	Signature hierSig;
	hierSig.set(registry.GetComponentID<Transform>());
	hierSig.set(registry.GetComponentID<Child>());
	registry.SetSystemSignature<HierarchySystem>(hierSig);

	//Set up level
	GenerateLevelBlocks();
	Prefab::GameObject::LevelBorders(registry, m_Game->GetTextureManager());
	Prefab::GameObject::KillZone(registry);

	Entity liveRemText = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(liveRemText, sf::Vector2f{ 25,25 });
	registry.AddComponentToEntity<Renderable>(liveRemText, nullptr, RenderLayer::UI);
	registry.AddComponentToEntity<RendText>(liveRemText, RendText{ "Lives Remaining:", &m_Game->GetFontManager().GetFont("pala") });

	m_StageEnts.lifeInd1Ent = Prefab::UI::LifeIndicator(registry, m_Game->GetTextureManager(), 1);
	m_StageEnts.lifeInd2Ent = Prefab::UI::LifeIndicator(registry, m_Game->GetTextureManager(), 2);

	Entity background = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(background, sf::Vector2f{ 0,0 }, sf::Vector2f(DefaultResolution));
	registry.AddComponentToEntity<Renderable>(background, &m_Game->GetTextureManager().Load("Forest_Background"), RenderLayer::Background);
	Entity overlay = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(overlay, sf::Vector2f{ 270,0 }, sf::Vector2f({ 720,720 }));
	registry.AddComponentToEntity<Renderable>(overlay, &m_Game->GetTextureManager().Load("Overlay"), RenderLayer::Background);

	//Set Ball and Paddle
	m_StageEnts.ballEnt = Prefab::GameObject::Ball(registry, m_Game->GetTextureManager());
	m_StageEnts.paddleEnt = Prefab::GameObject::Paddle(registry, m_Game->GetTextureManager());
	AttachBallToPaddle();

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
		if (m_LevelData.gameOverTimer > 0) {
			m_LevelData.gameOverTimer -= deltaTime;
			deltaTime *= (m_LevelData.gameOverTimer / 2.5f);
		}
		else if (!m_LevelData.gameWon && m_LevelData.livesLeft > 0) {
			m_LevelData.gameOverEnqueued = false;
			m_LevelData.livesLeft -= 1;

			if (m_LevelData.livesLeft == 1) registry.DestroyEntity(m_StageEnts.lifeInd2Ent);
			else registry.DestroyEntity(m_StageEnts.lifeInd1Ent);

			AttachBallToPaddle();
		}
		else {
			if (!m_LevelData.gameOver) {
				if (m_LevelData.gameWon) GameWon();
				else GameLost();
				m_LevelData.gameOver = true;
			}
			else {
				m_UISystem.Update(playerInput);
				return;
			}
		}
	}

	if (!m_LevelData.gamePaused) {
		if (playerInput.pauseGame && !m_LevelData.gameOverEnqueued) {
			playerInput.resumeGame = false;
			PauseGame();
			return;
		}

		UpdatePaddle(playerInput);
		if (playerInput.mouseClicked && m_LevelData.ballAttached) LaunchBall();
		m_CollisionSystem.Update();
		m_PhysicsSystem.Update(deltaTime);
		m_BlockSystem.Update();
		m_HierarchySystem.Update();

		for (auto& event : registry.GetEventQueue().GetTEvents<GameStateEvent>()) {
			if (event->type == GameStateEvent::Type::GameOver && !m_LevelData.gameOverEnqueued) {
				m_LevelData.gameOverEnqueued = true;
				m_LevelData.gameOverTimer = 2.5f;
				registry.GetEventQueue().Publish<AudioEvent>({AudioAsset::FX_Slowmo});		

				if (event->payload == 1) { //Stage won
					m_LevelData.gameWon = true;
				}
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

void PlayingState::PauseGame() {
	m_LevelData.gamePaused = true;

	Registry& registry = m_Game->GetRegistry();

	Entity darkOverlay = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(darkOverlay, Transform{ {0,0}, DefaultResolution });
	registry.AddComponentToEntity<Renderable>(darkOverlay, &m_Game->GetTextureManager().Load("Overlay"), RenderLayer::UI);
	m_OverlayEnt.push_back(darkOverlay);

	Entity pauseTitle = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(pauseTitle, sf::Vector2f{ 340,150 }, sf::Vector2f{ 600,50 });
	registry.AddComponentToEntity<Renderable>(pauseTitle, &m_Game->GetTextureManager().Load("Game_Paused"), RenderLayer::UI);
	m_OverlayEnt.push_back(pauseTitle);

	Entity continueBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(continueBtn, sf::Vector2f{ 440,300 }, sf::Vector2f{ 400,50 });
	registry.AddComponentToEntity<Renderable>(continueBtn, &m_Game->GetTextureManager().Load("UI/Continue"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(continueBtn, ButtonAction::ResumeGame);
	m_OverlayEnt.push_back(continueBtn);

	Entity restartBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(restartBtn, sf::Vector2f{ 390,400 }, sf::Vector2f{ 500,50 });
	registry.AddComponentToEntity<Renderable>(restartBtn, &m_Game->GetTextureManager().Load("UI/Restart"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(restartBtn, ButtonAction::RestartGame, m_LevelData.levelNumber);
	m_OverlayEnt.push_back(restartBtn);

	Entity backToMenuBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(backToMenuBtn, sf::Vector2f{ 390,500 }, sf::Vector2f{ 500,50 });
	registry.AddComponentToEntity<Renderable>(backToMenuBtn, &m_Game->GetTextureManager().Load("UI/Back_To_Main"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(backToMenuBtn, ButtonAction::MainMenu);
	m_OverlayEnt.push_back(backToMenuBtn);

	auto volCtrlEntites = Prefab::UI::VolumeControl(registry, m_Game->GetTextureManager(), m_Game->GetAudioManager().getCurVolume());
	m_OverlayEnt.insert(m_OverlayEnt.end(), volCtrlEntites.begin(), volCtrlEntites.end());

	m_Game->SetMouseVisibility(true);
}

void PlayingState::ResumeGame() {
	m_LevelData.gamePaused = false;
	Registry& registry = m_Game->GetRegistry();

	for (Entity ent : m_OverlayEnt) {
		registry.DestroyEntity(ent);
	}
	m_OverlayEnt.clear();
	m_Game->SetMouseVisibility(false);
}

void PlayingState::GameLost() {
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
	registry.AddComponentToEntity<Button>(restartLvlBtn, ButtonAction::StartGame, m_LevelData.levelNumber);

	Entity backToMenuBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(backToMenuBtn, sf::Vector2f{ 390,400 }, sf::Vector2f{ 500,50 });
	registry.AddComponentToEntity<Renderable>(backToMenuBtn, &m_Game->GetTextureManager().Load("UI/Back_To_Main"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(backToMenuBtn, ButtonAction::MainMenu);

	m_Game->SetMouseVisibility(true);
}

void PlayingState::GameWon() {
	Registry& registry = m_Game->GetRegistry();

	Entity darkOverlay = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(darkOverlay, Transform{ {0,0}, DefaultResolution });
	registry.AddComponentToEntity<Renderable>(darkOverlay, &m_Game->GetTextureManager().Load("Overlay"), RenderLayer::UI);

	Entity stageClearedTitle = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(stageClearedTitle, sf::Vector2f{ 340,150 }, sf::Vector2f{ 600,50 });
	registry.AddComponentToEntity<Renderable>(stageClearedTitle, &m_Game->GetTextureManager().Load("Stage_Cleared"), RenderLayer::UI);

	if (m_LevelData.levelNumber < 4) {
		Entity nextLvlBtn = registry.CreateEntity();
		registry.AddComponentToEntity<Transform>(nextLvlBtn, sf::Vector2f{ 390,300 }, sf::Vector2f{ 500,50 });
		registry.AddComponentToEntity<Renderable>(nextLvlBtn, &m_Game->GetTextureManager().Load("UI/Next_Level"), RenderLayer::UI);
		registry.AddComponentToEntity<Button>(nextLvlBtn, ButtonAction::StartGame, m_LevelData.levelNumber + 1);
	}

	Entity restartLvlBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(restartLvlBtn, sf::Vector2f{ 390,400 }, sf::Vector2f{ 500,50 });
	registry.AddComponentToEntity<Renderable>(restartLvlBtn, &m_Game->GetTextureManager().Load("UI/Restart"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(restartLvlBtn, ButtonAction::StartGame, m_LevelData.levelNumber);

	Entity backToMenuBtn = registry.CreateEntity();
	registry.AddComponentToEntity<Transform>(backToMenuBtn, sf::Vector2f{ 390,500 }, sf::Vector2f{ 500,50 });
	registry.AddComponentToEntity<Renderable>(backToMenuBtn, &m_Game->GetTextureManager().Load("UI/Back_To_Main"), RenderLayer::UI);
	registry.AddComponentToEntity<Button>(backToMenuBtn, ButtonAction::MainMenu);

	m_Game->SetMouseVisibility(true);
}

void PlayingState::UpdatePaddle(InputState& inputState) {
	Transform& paddleTrans = m_Game->GetRegistry().GetComponentArray<Transform>().GetTComponent(m_StageEnts.paddleEnt);
	sf::Vector2f mousePos = inputState.mousePos;

	//Clamp paddle position within world border
	paddleTrans.position.x = std::min(980.f - paddleTrans.size.x, std::max(275.f, mousePos.x));
}

void PlayingState::AttachBallToPaddle() {
	Registry& registry = m_Game->GetRegistry();
	Transform& paddleTrans = registry.GetEntityComponent<Transform>(m_StageEnts.paddleEnt);
	Transform& ballTrans = registry.GetEntityComponent<Transform>(m_StageEnts.ballEnt);
	
	registry.AddComponentToEntity<Child>(m_StageEnts.ballEnt, m_StageEnts.paddleEnt,
		sf::Vector2f{ (paddleTrans.size.x / 2) - (ballTrans.size.x / 2), -25 });

	m_LevelData.ballAttached = true;
}

void PlayingState::LaunchBall() {
	Registry& registry = m_Game->GetRegistry();
	m_LevelData.ballAttached = false;
	
	registry.RemoveComponentFromEntity<Child>(m_StageEnts.ballEnt);
	float ballSpeed = 800.f;

	Transform& ballTrans = registry.GetEntityComponent<Transform>(m_StageEnts.ballEnt);
	//Launch ball very slightly left or right-wards to prevent continuous straight bouncing
	float launchAngle = static_cast<int>(ballTrans.position.x) % 2 == 0 ? 269.95 : 270.05;

	registry.AddComponentToEntity<Physics>(m_StageEnts.ballEnt,
		sf::Vector2f{ ballSpeed, sf::Angle(sf::degrees(launchAngle)) });
}