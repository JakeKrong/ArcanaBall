#include "RenderSystem.h"

#include <algorithm>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Registry.h"

void RenderSystem::Update(sf::RenderWindow& renderWindow){
	if (!m_Registry || m_Entities.size() == 0) {
		return;
	}

	renderWindow.clear();

	auto& rendCompArr = m_Registry->GetComponentArray<Renderable>();
	auto& transCompArr = m_Registry->GetComponentArray<Transform>();
	
	//Sort array based on RenderLayer when change detecteed
	//if (m_PrevLastElement != m_Entities[m_Entities.size() - 1] || m_PrevElementCnt != m_Entities.size()) {
	if (m_PrevLastElement != m_Entities[m_Entities.size() - 1]) {

		std::ranges::sort(m_Entities, {}, [&](Entity ent) { return rendCompArr.GetTComponent(ent).layer; });
		m_PrevLastElement = m_Entities[m_Entities.size() - 1];
		//m_PrevElementCnt = m_Entities.size();
	}

	for (const Entity& ent : m_Entities) {
		auto& transComp = transCompArr.GetTComponent(ent);
		auto& rendComp = rendCompArr.GetTComponent(ent);

		if (!rendComp.visible) continue;

		//Create renderable for Render Window
		const sf::Texture* texture = rendComp.texture;
		if (rendComp.texture) {
			sf::RectangleShape renderable(transComp.size);
			renderable.setTexture(texture);
			renderable.setPosition(transComp.position);
			renderable.setRotation(sf::degrees(transComp.rotation));

			sf::IntRect texRect;
			if (m_Registry->EntityHasComponent<AnimationData>(ent)) {
				texRect = m_Registry->GetEntityComponent<AnimationData>(ent).activeSprite;
			}
			else {
				texRect = sf::IntRect({ 0, 0 }, 
					{ static_cast<int>(texture->getSize().x), static_cast<int>(texture->getSize().y) });
			}

			if (rendComp.flipX) {
				texRect.position.x = texRect.position.x + texRect.size.x;
				texRect.size.x = -texRect.size.x;
			}
			if (rendComp.flipY) {
				texRect.position.y = texRect.position.y + texRect.size.y;
				texRect.size.y = -texRect.size.y;
			}

			renderable.setTextureRect(texRect);
			renderWindow.draw(renderable);
		}

		if (m_Registry->EntityHasComponent<RendText>(ent)) {
			RendText& textComp = m_Registry->GetEntityComponent<RendText>(ent);
			sf::Text text(*textComp.font, textComp.text, textComp.size);
			text.setFillColor(textComp.color);
			text.setPosition(transComp.position);
			renderWindow.draw(text);
		}
	}

	renderWindow.display();
}