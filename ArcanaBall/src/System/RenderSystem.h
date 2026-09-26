#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "ISystem.h"

class RenderSystem : public ISystem {
public:
	void Update(sf::RenderWindow&);
};