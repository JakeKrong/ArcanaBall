#pragma once
#include <SFML/System/Vector2.hpp>
#include "Types.h"

struct Child {
	Entity parentEntity{ 0 };
	sf::Vector2f localOffset{ 0,0 };
};