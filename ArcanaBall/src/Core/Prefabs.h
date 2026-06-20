#pragma once

#include "Types.h"
#include "Registry.h"
#include "TextureManager.h"

namespace Prefab{
	namespace UI {
		std::array<Entity, 3> VolumeControl(Registry&, TextureManager&, float);
		Entity LifeIndicator(Registry&, TextureManager&, int);
	}
	
	namespace GameObject {
		Entity Ball(Registry&, TextureManager&);
		Entity Paddle(Registry&, TextureManager&);

		Entity Stone(Registry&, TextureManager&, sf::Vector2f);
		Entity Brick(Registry&, TextureManager&, sf::Vector2f);
		Entity Wood(Registry&, TextureManager&, sf::Vector2f);
		Entity Steel(Registry&, TextureManager&, sf::Vector2f);

		Entity WoodBreak(Registry&, TextureManager&, sf::Vector2f);

		void LevelBorders(Registry&, TextureManager&);
		Entity KillZone(Registry&);
	}
};