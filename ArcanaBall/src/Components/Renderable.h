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