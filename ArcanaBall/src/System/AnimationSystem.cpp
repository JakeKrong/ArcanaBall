#include <AnimationSystem.h>
#include "Registry.h"

void AnimationSystem::Update(float deltaTime) {
	auto& rendCompArr = m_Registry->GetComponentArray<Renderable>();
	auto& animCompArr = m_Registry->GetComponentArray<AnimationData>();

	auto entities = m_Entities; //Copy vector for stable iteration
	for (Entity ent : entities) {
		auto& rendComp = rendCompArr.GetTComponent(ent);
		auto& animComp = animCompArr.GetTComponent(ent);

		animComp.timeSinceLastFrame += deltaTime;

		if (animComp.timeSinceLastFrame > animComp.frameTime || !animComp.currentFrame){
			animComp.timeSinceLastFrame -= animComp.frameTime; //Reset time counter

			if (animComp.currentFrame + 1 > animComp.totalFrame && !animComp.loop) {
				m_Registry->DestroyEntity(ent);
			}
			else {
				//Set new frame
				if (animComp.currentFrame + 1 > animComp.totalFrame && animComp.loop) animComp.currentFrame = 1;
				else if (!animComp.currentFrame) { //Newly initiated
					animComp.currentFrame = 1;
					animComp.timeSinceLastFrame = 0.f;
				}
				else animComp.currentFrame++;

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
				int i = 1;
			}
		}
	}
}