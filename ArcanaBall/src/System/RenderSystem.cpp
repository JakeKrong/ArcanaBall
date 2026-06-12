#include "RenderSystem.h"

#include <algorithm>
#include <SFML/Graphics/Sprite.hpp>
#include "Registry.h"

void RenderSystem::Update(sf::RenderWindow& renderWindow){
	if (!m_Registry || m_Entities.size() == 0) {
		return;
	}

	renderWindow.clear();

	auto& rendCompArr = m_Registry->GetComponentArray<Renderable>();
	auto& transCompArr = m_Registry->GetComponentArray<Transform>();
	
	//Sort array based on RenderLayer when change detecteed
	if (m_PrevLastElement != m_Entities[m_Entities.size() - 1] || m_PrevElementCnt != m_Entities.size()) {

		std::ranges::sort(m_Entities, {}, [&](Entity ent) { return m_Entities.at(ent); });
		m_PrevLastElement = m_Entities[m_Entities.size() - 1];
		m_PrevElementCnt = m_Entities.size();
	}

	for (const Entity& ent : m_Entities) {
		auto& transComp = transCompArr.GetTComponent(ent);
		auto& rendComp = rendCompArr.GetTComponent(ent);

		if (!rendComp.visible) continue;

		//Create renderable for Render Window
		const sf::Texture* texture = rendComp.texture;
		if (rendComp.texture) {
			sf::Sprite renderable(*rendComp.texture);
			renderable.setPosition(transComp.position);
			renderable.setRotation(sf::degrees(transComp.rotation));
			renderable.setScale({ transComp.size.x / texture->getSize().x,
								  transComp.size.y / texture->getSize().y });
			renderWindow.draw(renderable);
		}
	}

	renderWindow.display();
}