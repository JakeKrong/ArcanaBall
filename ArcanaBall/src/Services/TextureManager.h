#pragma once
#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>

class TextureManager {
public:
	sf::Texture& Load(const std::string& filePath); //Consider using enums later on
	
	void InitTexture();

private:
	std::unordered_map<std::string, sf::Texture> m_TextureMap; //Consider using enums later on

#ifdef _DEBUG
	std::string m_TextureBasePath = "../../../../ArcanaBall/assets/textures/";
#else
	std::string m_TextureBasePath = "assets/textures/";
#endif
};