#pragma once
#include "ISystem.h"

class PhysicsSystem : public ISystem {
public:
	void Step(float stepTime);
};