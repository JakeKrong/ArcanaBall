#include <TextureManager.h>

//Debugging
#include <print>

sf::Texture& TextureManager::Load(const std::string& path) {
	auto it = m_TextureMap.find(path);

	if (it != m_TextureMap.end()) {
		return it->second;
	}

	sf::Texture texture;
	if (!texture.loadFromFile(m_TextureBasePath + path + ".png")) {
		std::println("[Texture Manager] Failed to load texture from {}!", path);
	}

	return m_TextureMap[path] = std::move(texture);
}