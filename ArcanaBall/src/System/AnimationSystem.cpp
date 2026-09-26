#include <AnimationSystem.h>
#include "Registry.h"

void AnimationSystem::Update(float deltaTime) {
	auto& rendCompArr = m_Registry->GetComponentArray<Renderable>();
	auto& animCompArr = m_Registry->GetComponentArray<AnimationData>();

	for (Entity ent : m_Entities) {
		if (!m_Registry->IsEntityLive(ent)) continue;

		auto& rendComp = rendCompArr.GetTComponent(ent);
		auto& animComp = animCompArr.GetTComponent(ent);

		animComp.timeSinceLastFrame += deltaTime;

		const int prevFrame = animComp.currentFrame;
		bool animEnded = false;

		if (!animComp.currentFrame) {	//Newly created animation
			animComp.currentFrame = 1;
			animComp.timeSinceLastFrame = 0.f;
		}
		else if (animComp.frameTime > 0.f) {
			while (animComp.timeSinceLastFrame > animComp.frameTime) {
				animComp.timeSinceLastFrame -= animComp.frameTime;

				if (animComp.currentFrame + 1 > animComp.totalFrame) {
					if (!animComp.loop) {	//If animation ended, and is not set to loop
						m_Registry->DestroyEntity(ent);
						animEnded = true;
						break;
					}
					animComp.currentFrame = 1;
				}
				else animComp.currentFrame++;
			}
		}

		if (animEnded || animComp.currentFrame == prevFrame) continue;

		//Update ActiveSpriteRect
		const sf::Vector2u& textureDim = rendComp.texture->getSize();

		sf::Vector2u frameSize = { textureDim.x / animComp.spriteSheetDim.x,
								   textureDim.y / animComp.spriteSheetDim.y };

		int column = (animComp.currentFrame - 1) % animComp.spriteSheetDim.x;
		int row = (animComp.currentFrame - 1) / animComp.spriteSheetDim.x;

		animComp.activeSprite = {
			{ column * static_cast<int>(frameSize.x), row * static_cast<int>(frameSize.y) },
			{ static_cast<int>(frameSize.x), static_cast<int>(frameSize.y) }
		};
	}
}
