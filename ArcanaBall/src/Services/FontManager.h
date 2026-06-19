#pragma once
#include <SFML/Graphics/Font.hpp>
#include <unordered_map>

class FontManager {
public:
	sf::Font& GetFont(const std::string& fontName);

private:
	std::unordered_map<std::string, sf::Font> m_FontMap;

#ifdef _DEBUG
	std::string fontBasePath{ "../../../../ArcanaBall/assets/font/" };
#else 
	std::string fontBasePath = "assets/font/";
#endif
};