#pragma once
#include "ISystem.h"

class AnimationSystem : public ISystem {
public:
	void Update(float deltaTime);
};