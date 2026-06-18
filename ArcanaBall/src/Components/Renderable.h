#pragma once
#include <cstdint>
#include <limits>
#include <SFML/Graphics/Texture.hpp>

enum RenderLayer : uint8_t{
	Background,
	GameObjects,
	VFX,
	UI
};

struct Renderable {

	const sf::Texture* texture = nullptr;
	RenderLayer layer{ 0 };
	bool visible{ true };
	bool flipX{ false };
};

struct AnimationData {
	sf::IntRect activeSprite{}; //Check if can use diffenrent data type
	sf::Vector2i spriteSheetDim{ 1,1 };

	float frameTime = 1.0f;
	int totalFrame = 1;
	int currentFrame = 1;
	float timeSinceLastFrame = std::numeric_limits<float>::max();

};