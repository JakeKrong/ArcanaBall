#pragma once

#include "Types.h"
#include "Registry.h"
#include "TextureManager.h"
#include "FontManager.h"

namespace Prefab{
	namespace UI {
		std::array<Entity, 3> VolumeControl(Registry&, TextureManager&, float);
		Entity LifeIndicator(Registry&, TextureManager&, int);
		void Controls(Registry&, TextureManager&, FontManager&);
	}
	
	namespace GameObject {
		Entity Ball(Registry&, TextureManager&);
		Entity Paddle(Registry&, TextureManager&);

		Entity Stone(Registry&, TextureManager&, sf::Vector2f);
		Entity Brick(Registry&, TextureManager&, sf::Vector2f);
		Entity Wood(Registry&, TextureManager&, sf::Vector2f);
		Entity Steel(Registry&, TextureManager&, sf::Vector2f);

		Entity BlockBreakEff(Registry&, TextureManager&, sf::Vector2f, BlockType);
		Entity BallElementEff(Registry&, TextureManager&, Entity, sf::Vector2f, ElemInfusion);
		void BlockElementEff(Registry&, TextureManager&, Entity, sf::Vector2f, ElemInfusion);

		void LevelBorders(Registry&, TextureManager&);
		Entity KillZone(Registry&);
	}

	namespace Reaction{
		void IceShatter(Registry&, TextureManager&, sf::Vector2f);
		void Overload(Registry&, TextureManager&, sf::Vector2f);
		void LightningCross(Registry&, TextureManager&, sf::Vector2f);
	}
};