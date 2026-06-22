#pragma once
#include <SFML/Graphics/Font.hpp>
#include <unordered_map>
#include "Types.h"

class FontManager {
public:
	sf::Font& GetFont(const std::string& fontName);

private:
	std::unordered_map<std::string, sf::Font> m_FontMap;

#if BuildForPlayable
	std::string fontBasePath = "assets/font/";
#else 
	std::string fontBasePath{ "../../../../ArcanaBall/assets/font/" };
#endif
};