#pragma once
#include "ISystem.h"

class PhysicsSystem : public ISystem {
public:
	void Update(float);

private:
	float timeSinceFixedUpdate = 0;
};