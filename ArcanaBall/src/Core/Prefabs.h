#pragma once

#include "Types.h"
#include "Registry.h"
#include "TextureManager.h"

namespace Prefab{
	Entity Button(Registry&, TextureManager&);
	Entity Ball(Registry&, TextureManager&);
	Entity Brick(Registry& reg, TextureManager& textMn, sf::Vector2f);
	Entity Wood(Registry& reg, TextureManager& textMn, sf::Vector2f);
	void LevelBorders(Registry&, TextureManager&);
};