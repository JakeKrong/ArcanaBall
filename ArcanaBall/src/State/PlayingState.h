#pragma once
#include "IState.h"
#include "Systems.h"

class PlayingState : public IState {
public:
	explicit PlayingState(Game*, int levelNumber, StageGridData& stageData);

	void Enter() override;
	void Exit() override;

	void Update(float) override;
	void Render(sf::RenderWindow&) override;

	// Game State Functions //
	void GenerateLevelBlocks();
	void PauseGame();
	void ResumeGame();
	void GameLost();
	void GameWon();

	// Gameplay Functions //
	void UpdatePaddle(InputState&);
	void AttachBallToPaddle();
	void LaunchBall();

private:
	// Systems //
	RenderSystem& m_RenderSystem;
	UISystem& m_UISystem;
	CollisionSystem& m_CollisionSystem;
	PhysicsSystem& m_PhysicsSystem;
	BlockSystem& m_BlockSystem;
	HierarchySystem& m_HierarchySystem;

	struct LevelData {
		StageGridData& stageGrid;
		int levelNumber{ 0 };
		int livesLeft{ 2 };
		bool ballAttached = true;

		float gameOverTimer{ 0 };
		bool gamePaused = false;
		bool gameOverEnqueued = false;
		bool gameOver = false;
		bool gameWon = false;
	};

	struct StageEnts {
		Entity paddleEnt{ 0 };
		Entity ballEnt{ 0 };

		Entity lifeInd1Ent{ 0 };
		Entity lifeInd2Ent{ 0 };
	};

	LevelData m_LevelData;
	StageEnts m_StageEnts;
	std::vector<Entity> m_OverlayEnt;
};