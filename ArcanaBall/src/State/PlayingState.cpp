#include "PlayingState.h"
#include "Game.h"
#include "Prefabs.h"

PlayingState::PlayingState(Game* game, StageGridData& stageData) :
	IState(game),
	m_RenderSystem(game->GetRegistry().RegisterSystem<RenderSystem>()),
	m_UISystem(game->GetRegistry().RegisterSystem<UISystem>()),
	m_CollisionSystem(game->GetRegistry().RegisterSystem<CollisionSystem>()),
	m_PhysicsSystem(game->GetRegistry().RegisterSystem<PhysicsSystem>()),
	m_StageGrid(stageData)
{}

void PlayingState::Enter() {
	Registry& registry = m_Game->GetRegistry();

	// *** Components ****//
	//Register
	registry.RegisterComponent<Transform>();
	registry.RegisterComponent<Renderable>();
	registry.RegisterComponent<Button>();
	registry.RegisterComponent<Collider>();
	registry.RegisterComponent<Physics>();

	// *** Systems ****//
	//Set Registry
	m_RenderSystem.SetRegistry(&m_Game->GetRegistry());
	m_UISystem.SetRegistry(&m_Game->GetRegistry());
	m_CollisionSystem.SetRegistry(&m_Game->GetRegistry());
	m_PhysicsSystem.SetRegistry(&m_Game->GetRegistry());

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

	//Set up level
	GenerateLevelBlocks();
	Prefab::LevelBorders(registry, m_Game->GetTextureManager());
	Prefab::Ball(registry, m_Game->GetTextureManager());

	m_CollisionSystem.RegisterCollisionHandlers();
	m_CollisionSystem.InitBlockGridMap();
}

void PlayingState::Exit() {

}

void PlayingState::Update(float deltaTime) {
	auto& playerInput = m_Game->GetInputManager().GetInputStates();

	m_UISystem.Update(playerInput);
	m_CollisionSystem.Update();
	m_PhysicsSystem.Update(deltaTime);
}

void PlayingState::Render(sf::RenderWindow& renderWindow) {
	m_RenderSystem.Update(renderWindow);
}

void PlayingState::GenerateLevelBlocks() {
	Registry& registry = m_Game->GetRegistry();
	TextureManager& textureMn= m_Game->GetTextureManager();

	float yPosition = GRID_OFFSET_Y, xPosition = GRID_OFFSET_X;

	for (auto rows : m_StageGrid) {
		for (auto col : rows){
			switch (col) {
			case 1:
				Prefab::Brick(registry, textureMn, sf::Vector2f{xPosition, yPosition});
				break;
			case 2:
				Prefab::Wood(registry, textureMn, sf::Vector2f{ xPosition, yPosition });
				break;
			default:
				continue;
			}
			xPosition += BLOCK_WIDTH;
		}
		//Reset X position, move Y postion down
		xPosition = GRID_OFFSET_X;
		yPosition += BLOCK_HEIGHT;
	}
}