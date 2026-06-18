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


	// Gameplay Functions //
	void GenerateLevelBlocks();
	void UpdatePaddle(InputState&);

	void PauseGame();
	void ResumeGame();
	void GameOver();

private:
	// Systems //
	RenderSystem& m_RenderSystem;
	UISystem& m_UISystem;
	CollisionSystem& m_CollisionSystem;
	PhysicsSystem& m_PhysicsSystem;
	BlockSystem& m_BlockSystem;

	struct LevelData {
		StageGridData& stageGrid;
		int levelNumber{ 0 };

		int livesLeft{ 2 };

		Entity paddleEnt{ 0 };
		Entity ballEnt{ 0 };

		float gameOverTimer{ 0 };
		bool gamePaused = false;
		bool gameOverEnqueued = false;
		bool gameOver = false;

		std::vector<Entity> overlayEnt;
	};

	LevelData m_LevelData;
};