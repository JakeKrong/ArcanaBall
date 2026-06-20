#pragma once

struct AnimationData {
	AnimationData() = default;

	AnimationData(sf::Vector2i spriteSheetDim, int totalFrame, float frameTime, bool loop)  :
		spriteSheetDim(spriteSheetDim),
		totalFrame(totalFrame),
		frameTime(frameTime),
		loop(loop)
	{}

	sf::IntRect activeSprite{};
	sf::Vector2i spriteSheetDim{ 1,1 };

	float frameTime = 1.0f;
	int totalFrame = 1;
	int currentFrame = 0;
	float timeSinceLastFrame = 0.f;
	bool loop = false;
};