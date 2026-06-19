#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "ISystem.h"

class RenderSystem : public ISystem {
public:
	void Update(sf::RenderWindow&);

private:
	Entity m_PrevLastElement = 0;
	//int m_PrevElementCnt = 0;
};