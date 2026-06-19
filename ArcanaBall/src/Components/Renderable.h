#pragma once
#include <cstdint>
#include <limits>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>

enum RenderLayer : uint8_t {
	Background,
	GameObjects,
	VFX,
	UI,
	Text
};

struct Renderable {
	const sf::Texture* texture = nullptr;
	RenderLayer layer{ 0 };
	bool visible{ true };
	bool flipX{ false };
};

struct RendText {
	RendText() = default;
	RendText(std::string text, sf::Font* font) :
		text(std::move(text)),
		font(font)
	{};
	std::string text;
	const sf::Font* font = nullptr;
	sf::Color color = sf::Color::White;
	int size = 24;
};

struct AnimationData {
	sf::IntRect activeSprite{}; //Check if can use diffenrent data type
	sf::Vector2i spriteSheetDim{ 1,1 };

	float frameTime = 1.0f;
	int totalFrame = 1;
	int currentFrame = 1;
	float timeSinceLastFrame = std::numeric_limits<float>::max();
};