#pragma once
#include <SFML/System/Vector2.hpp>

#include "ISystem.h"
#include "InputManager.h"

class UISystem : public ISystem {
public:
	void Update(const InputState&);

	void SetButtonsEnabled(bool);
};