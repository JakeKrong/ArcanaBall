#pragma once
#include "BaseEvent.h"
#include <SFML/System/Vector2.hpp>

struct PhysicsEvent : BaseEvent {
	Entity ent = 0;
	sf::Vector2f vectorChange{ 0,0 };
	bool inverseX = false;
	bool inverseY = false;
	bool nudgeWithDelta = false;
};