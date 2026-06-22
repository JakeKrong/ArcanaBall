#include "Prefabs.h"
#include "Registry.h"

#include "EntityDestroyedEvent.h"

std::array<Entity, 3> Prefab::UI::VolumeControl(Registry& reg, TextureManager& textMn, float currVolume) {
	Entity volumeIcon = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(volumeIcon, sf::Vector2f{ 960,25 }, sf::Vector2f{ 50,50 });
	reg.AddComponentToEntity<Renderable>(volumeIcon, &textMn.Load("UI/Volume"), RenderLayer::UI2);

	Entity volumeSlider = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(volumeSlider, sf::Vector2f{ 1025,35 }, sf::Vector2f{ 180,30 });
	reg.AddComponentToEntity<Renderable>(volumeSlider, &textMn.Load("UI/Slider"), RenderLayer::UI2);

	Entity sliderButton = reg.CreateEntity();
	float currSliderX = 1035.f + (currVolume / DefaultVolumeSetting * 75.f);
	reg.AddComponentToEntity<Transform>(sliderButton, sf::Vector2f{ currSliderX,35 }, sf::Vector2f{ 15,22 });
	reg.AddComponentToEntity<Renderable>(sliderButton, &textMn.Load("UI/Slider_Butt"), RenderLayer::UI2);
	reg.AddComponentToEntity<Button>(sliderButton, ButtonAction::AdjustVolume);

	return { volumeIcon, volumeSlider, sliderButton };
}

Entity Prefab::UI::LifeIndicator(Registry& reg, TextureManager& textMn, int numbering) {
	Entity lifeInd = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(lifeInd, numbering == 1 ? sf::Vector2f{ 30,65 } : sf::Vector2f{ 60,65 }, sf::Vector2f{ 25, 25 });
	reg.AddComponentToEntity<Renderable>(lifeInd, &textMn.Load("GameObject/Ball"), RenderLayer::UI);
	return lifeInd;
}

void Prefab::UI::Controls(Registry& reg, TextureManager& textMn, FontManager& fontMn) {
	Entity controlFrame = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(controlFrame, sf::Vector2f{ 12.5,150 }, sf::Vector2f{ 230, 382 });
	reg.AddComponentToEntity<Renderable>(controlFrame, &textMn.Load("UI/Controls"), RenderLayer::VFX);

	Entity infuseFireText = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(infuseFireText, sf::Vector2f{ 80,230 });
	reg.AddComponentToEntity<Renderable>(infuseFireText, nullptr, RenderLayer::UI);
	reg.AddComponentToEntity<RendText>(infuseFireText, "Fire Infusion", &fontMn.GetFont("pala"), 22, sf::Color::Red);

	Entity infuseIceText = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(infuseIceText, sf::Vector2f{ 80,275 });
	reg.AddComponentToEntity<Renderable>(infuseIceText, nullptr, RenderLayer::UI);
	reg.AddComponentToEntity<RendText>(infuseIceText, "Ice Infusion", &fontMn.GetFont("pala"), 22, sf::Color::Cyan);

	Entity infuseLightningText = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(infuseLightningText, sf::Vector2f{ 80,310});
	reg.AddComponentToEntity<Renderable>(infuseLightningText, nullptr, RenderLayer::UI);
	reg.AddComponentToEntity<RendText>(infuseLightningText, "Lightning\nInfusion", &fontMn.GetFont("pala"), 22, sf::Color::Magenta);

	Entity launchBallText = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(launchBallText, sf::Vector2f{ 80,385 });
	reg.AddComponentToEntity<Renderable>(launchBallText, nullptr, RenderLayer::UI);
	reg.AddComponentToEntity<RendText>(launchBallText, "Launch Ball", &fontMn.GetFont("pala"), 22);

	Entity pauseGameText = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(pauseGameText, sf::Vector2f{ 85,450 });
	reg.AddComponentToEntity<Renderable>(pauseGameText, nullptr, RenderLayer::UI);
	reg.AddComponentToEntity<RendText>(pauseGameText, "Pause Game", &fontMn.GetFont("pala"), 22);

}

Entity Prefab::GameObject::Paddle(Registry& reg, TextureManager& textMn) {
	Entity paddle = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(paddle, sf::Vector2f{ 640,625 }, sf::Vector2f{ 110,15 });
	reg.AddComponentToEntity<Renderable>(paddle, &textMn.Load("GameObject/Paddle"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(paddle, ColliderType::Paddle, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Physics>(paddle, sf::Vector2f{ 0,0 });
	reg.AddComponentToEntity<StatusEffect>(paddle);

	return paddle;
}

Entity Prefab::GameObject::Ball(Registry& reg, TextureManager& textMn) {
	Entity ball = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(ball, sf::Vector2f{ 640,560 }, sf::Vector2f{ 20,20 });
	reg.AddComponentToEntity<Renderable>(ball, &textMn.Load("GameObject/Ball"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(ball, ColliderType::Ball, ColliderShape::Circle);
	reg.AddComponentToEntity<StatusEffect>(ball, ElemInfusion::None);
	reg.AddComponentToEntity<Child>(ball, Entity{ 0 });

	return ball;
}

Entity Prefab::GameObject::BallElementEff(Registry& reg, TextureManager& textMn, Entity ballEnt, sf::Vector2f ballPos, ElemInfusion elem){
	Entity ballEff = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(ballEff, ballPos, sf::Vector2f{ 20,20 });
	switch (elem) {
	case(ElemInfusion::Fire):
		reg.AddComponentToEntity<Renderable>(ballEff, &textMn.Load("Effects/Fire_Ball_Overlay"), RenderLayer::VFX);
		reg.AddComponentToEntity<AnimationData>(ballEff, sf::Vector2i{ 3,2 }, 6, .125f, true);
		break;
	case(ElemInfusion::Ice):
		reg.AddComponentToEntity<Renderable>(ballEff, &textMn.Load("Effects/Ice_Ball_Overlay"), RenderLayer::VFX);
		reg.AddComponentToEntity<AnimationData>(ballEff, sf::Vector2i{ 3,2 }, 6, .2f, true);
		break;
	case(ElemInfusion::Lightning):
		reg.AddComponentToEntity<Renderable>(ballEff, &textMn.Load("Effects/Lightning_Ball_Overlay"), RenderLayer::VFX);
		reg.AddComponentToEntity<AnimationData>(ballEff, sf::Vector2i{ 3,2 }, 6, .1f, true);
		break;
	}
	reg.AddComponentToEntity<Child>(ballEff, Entity{ ballEnt });

	return ballEff;
}

// ********** BLOCKS ********** //

Entity Prefab::GameObject::Stone(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity stone = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(stone, position, sf::Vector2f{ BLOCK_WIDTH,BLOCK_HEIGHT });
	reg.AddComponentToEntity<Renderable>(stone, &textMn.Load("GameObject/Stone"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(stone, ColliderType::Block, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Block>(stone, BlockType::Stone, 1.f,
		Block::ResistMatrix{ Block::ResistanceLvl::Vulnerable, Block::ResistanceLvl::Vulnerable, Block::ResistanceLvl::Vulnerable });
	reg.AddComponentToEntity<StatusEffect>(stone);

	return stone;
}

Entity Prefab::GameObject::Brick(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity brick = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(brick, position, sf::Vector2f{ BLOCK_WIDTH,BLOCK_HEIGHT });
	reg.AddComponentToEntity<Renderable>(brick, &textMn.Load("GameObject/Brick"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(brick, ColliderType::Block, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Block>(brick, BlockType::Brick, 4.f, 
		Block::ResistMatrix{Block::ResistanceLvl::Susceptible, Block::ResistanceLvl::Vulnerable, Block::ResistanceLvl::Susceptible });
	reg.AddComponentToEntity<StatusEffect>(brick);

	return brick;
}

Entity Prefab::GameObject::Wood(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity wood = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(wood, position, sf::Vector2f{ BLOCK_WIDTH,BLOCK_HEIGHT });
	reg.AddComponentToEntity<Renderable>(wood, &textMn.Load("GameObject/Wood"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(wood, ColliderType::Block, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Block>(wood, BlockType::Wood, 6.f, 
		Block::ResistMatrix{ Block::ResistanceLvl::Vulnerable, Block::ResistanceLvl::Immune, Block::ResistanceLvl::Susceptible });
	reg.AddComponentToEntity<StatusEffect>(wood);

	return wood;
}

Entity Prefab::GameObject::Steel(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity steel = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(steel, position, sf::Vector2f{ BLOCK_WIDTH,BLOCK_HEIGHT });
	reg.AddComponentToEntity<Renderable>(steel, &textMn.Load("GameObject/Steel"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(steel, ColliderType::Block, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Block>(steel, BlockType::Steel, 10.f, 
		Block::ResistMatrix{ Block::ResistanceLvl::Susceptible, Block::ResistanceLvl::Vulnerable, Block::ResistanceLvl::Vulnerable });
	reg.AddComponentToEntity<StatusEffect>(steel);

	return steel;
}

// Block breaking effects //
Entity Prefab::GameObject::BlockBreakEff(Registry& reg, TextureManager& textMn, sf::Vector2f position, BlockType blockType) {
	Entity breakEffect = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(breakEffect, position, sf::Vector2f{ BLOCK_WIDTH,BLOCK_HEIGHT });
	switch (blockType) {
	case(BlockType::Stone):
			reg.AddComponentToEntity<Renderable>(breakEffect, &textMn.Load("GameObject/Stone_Break"), RenderLayer::GameObjects);
			break;
	case(BlockType::Wood):
		reg.AddComponentToEntity<Renderable>(breakEffect, &textMn.Load("GameObject/Wood_Break"), RenderLayer::GameObjects);
		break;
	case(BlockType::Brick):
		reg.AddComponentToEntity<Renderable>(breakEffect, &textMn.Load("GameObject/Brick_Break"), RenderLayer::GameObjects);
		break;
	case(BlockType::Steel):
		reg.AddComponentToEntity<Renderable>(breakEffect, &textMn.Load("GameObject/Steel_Break"), RenderLayer::GameObjects);
		break;
	}
	reg.AddComponentToEntity<AnimationData>(breakEffect, sf::Vector2i{ 2,1 }, 2, .1f, false);

	return breakEffect;
}

void Prefab::GameObject::BlockElementEff(Registry& reg, TextureManager& textMn, Entity parentEnt, sf::Vector2f blockPos, ElemInfusion elem) {
	//Check if parent entity still valid
	if (!reg.EntityHasComponent<Transform>(parentEnt)) return;

	Entity blockEff = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(blockEff, blockPos, sf::Vector2f{ BLOCK_WIDTH, BLOCK_HEIGHT });
	switch (elem) {
	case(ElemInfusion::Fire):
		reg.AddComponentToEntity<Renderable>(blockEff, &textMn.Load("Effects/Fire_Block_Overlay"), RenderLayer::VFX);
		reg.AddComponentToEntity<AnimationData>(blockEff, sf::Vector2i{ 3,2 }, 6, .125f, true);
		break;
	case(ElemInfusion::Ice):
		reg.AddComponentToEntity<Renderable>(blockEff, &textMn.Load("Effects/Ice_Block_Overlay"), RenderLayer::VFX);
		reg.AddComponentToEntity<AnimationData>(blockEff, sf::Vector2i{ 3,2 }, 6, .2f, true);
		break;
	case(ElemInfusion::Lightning):
		reg.AddComponentToEntity<Renderable>(blockEff, &textMn.Load("Effects/Lightning_Block_Overlay"), RenderLayer::VFX);
		reg.AddComponentToEntity<AnimationData>(blockEff, sf::Vector2i{ 3,2 }, 6, .1f, true);
		break;
	}
	reg.AddComponentToEntity<Child>(blockEff, Entity{ parentEnt });
}

// *** Level Setup *** //
void Prefab::GameObject::LevelBorders(Registry& reg, TextureManager& textMn) {

	Entity leftWall = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(leftWall, sf::Vector2f{ 250,0 }, sf::Vector2f{ 25,720 });
	reg.AddComponentToEntity<Renderable>(leftWall, &textMn.Load("Level_Border"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(leftWall, ColliderType::Border, ColliderShape::Rectangle);

	Entity rightWall = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(rightWall, sf::Vector2f{ 1005,0 }, sf::Vector2f{ 25,720 });
	reg.AddComponentToEntity<Renderable>(rightWall, &textMn.Load("Level_Border"), RenderLayer::GameObjects, true, true);
	reg.AddComponentToEntity<Collider>(rightWall, ColliderType::Border, ColliderShape::Rectangle);

	Entity topWall = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(topWall, sf::Vector2f{ 275,0 }, sf::Vector2f{ 730,25 });
	reg.AddComponentToEntity<Renderable>(topWall, &textMn.Load("Level_Border_Top"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(topWall, ColliderType::Border, ColliderShape::Rectangle);

}

Entity Prefab::GameObject::KillZone(Registry& reg) {
	Entity killZone = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(killZone, sf::Vector2f{ 250,730 }, sf::Vector2f{ 755, 25 });
	reg.AddComponentToEntity<Collider>(killZone, ColliderType::OutZone, ColliderShape::Rectangle);

	return killZone;
}

// ********** REACTIONS ********** //
void Prefab::Reaction::IceShatter(Registry& reg, TextureManager& textMn, sf::Vector2f blockPos) {
	Entity shatterEffect = reg.CreateEntity();
	Transform transform;
	transform.position = { blockPos.x - (BLOCK_WIDTH / 2) , blockPos.y - (BLOCK_HEIGHT / 2) };
	transform.size = { BLOCK_WIDTH * 2, BLOCK_HEIGHT * 2 };
	reg.AddComponentToEntity<Transform>(shatterEffect, transform);
	reg.AddComponentToEntity<Renderable>(shatterEffect, &textMn.Load("Effects/Ice_Shatter"), RenderLayer::VFX);
	reg.AddComponentToEntity<AnimationData>(shatterEffect, sf::Vector2i{ 2,2 }, 4, .067f, false);

	Entity shatterCollider = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(shatterCollider, transform);
	reg.AddComponentToEntity<Collider>(shatterCollider, ColliderType::Effects, ColliderShape::Rectangle);
	reg.AddComponentToEntity<StatusEffect>(shatterCollider, ElemInfusion::Ice, ActiveReaction::IceShatter);

	//Assign the collider as child as the visual effect, and use deferred destroy child entity to get rid of collider after next frame
	reg.AddComponentToEntity<Child>(shatterCollider, shatterEffect);
	reg.GetEventQueue().PublishDeferred<DestroyChildEntity>(shatterEffect);
}

void Prefab::Reaction::Overload(Registry& reg, TextureManager& textMn, sf::Vector2f blockPos) {
	sf::Vector2f blockCenter = { blockPos.x + (BLOCK_WIDTH / 2), blockPos.y + (BLOCK_HEIGHT / 2) };

	for (int i = 0; i < 4; i++) {
		Entity overloadEffect = reg.CreateEntity();

		Transform transform;
		Renderable rend;

		transform.size = { BLOCK_HEIGHT * .95f, BLOCK_HEIGHT * .95f };

		switch (i) {
		case(0): //Left Explosion
			transform.position = { blockPos.x + 10 - transform.size.x, blockCenter.y - (transform.size.y / 2) };
			rend.texture = &textMn.Load("Effects/Overload_Horiz");
			break;
		case(1): //Right Explosion
			transform.position = { blockPos.x - 10 + BLOCK_WIDTH, blockCenter.y - (transform.size.y / 2) };
			rend.texture = &textMn.Load("Effects/Overload_Horiz");
			rend.flipX = true;
			break;
		case(2): //Upright Explosion
			transform.position = { blockCenter.x - (transform.size.x / 2) , blockPos.y + 10 - transform.size.y  };
			rend.texture = &textMn.Load("Effects/Overload");
			break;
		case(3): //Down Explosion
			transform.position = { blockCenter.x - (transform.size.x / 2) , blockPos.y - 10 + BLOCK_HEIGHT };
			rend.texture = &textMn.Load("Effects/Overload");
			rend.flipY = true;
			break;
		}
		rend.layer = RenderLayer::VFX;

		reg.AddComponentToEntity<Transform>(overloadEffect, transform);
		reg.AddComponentToEntity<Renderable>(overloadEffect, rend);
		reg.AddComponentToEntity<AnimationData>(overloadEffect, sf::Vector2i{ 4,2 }, 8, .055f, false);

		Entity overloadCollider = reg.CreateEntity();
		reg.AddComponentToEntity<Transform>(overloadCollider, transform);
		reg.AddComponentToEntity<Collider>(overloadCollider, ColliderType::Effects, ColliderShape::Rectangle);
		reg.AddComponentToEntity<StatusEffect>(overloadCollider, ElemInfusion::Fire, ActiveReaction::Overload);
		//Assign the collider as child as the visual effect, and use deferred destroy child entity to get rid of collider after next frame
		reg.AddComponentToEntity<Child>(overloadCollider, overloadEffect);
		reg.GetEventQueue().PublishDeferred<DestroyChildEntity>(overloadEffect);

		Entity extraEff = reg.CreateEntity();
		reg.AddComponentToEntity<Transform>(extraEff, blockPos, sf::Vector2f{BLOCK_WIDTH, BLOCK_HEIGHT});
		reg.AddComponentToEntity<Renderable>(extraEff, &textMn.Load("Effects/Overload"), RenderLayer::VFX);
		reg.AddComponentToEntity<AnimationData>(extraEff, sf::Vector2i{ 4,2 }, 8, .055f, false);
	}
}

void Prefab::Reaction::LightningCross(Registry& reg, TextureManager& textMn, sf::Vector2f blockPos) {
	sf::Vector2f blockCenter = { blockPos.x + (BLOCK_WIDTH / 2), blockPos.y + (BLOCK_HEIGHT / 2) };

	for (int i = 0; i < 4; i++) {
		Entity lightningEffect = reg.CreateEntity();

		Transform transform;
		Renderable rend;

		switch (i) {
		case(0): //Left Lightning
			transform.size = { 300, BLOCK_HEIGHT * .5f };
			transform.position = { blockPos.x - 300 - 1 , blockCenter.y - (transform.size.y / 2)};
			rend.texture = &textMn.Load("Effects/Lightning_Strike");
			rend.flipX = true;
			break;
		case(1): //Right Lightning
			transform.size = { 300, BLOCK_HEIGHT * .5f };
			transform.position = { blockPos.x + BLOCK_WIDTH + 1 , blockCenter.y - (transform.size.y / 2) };
			rend.texture = &textMn.Load("Effects/Lightning_Strike");
			break;
		case(2): //Upright Lightning
			transform.size = { BLOCK_HEIGHT * .5f, 300 };
			transform.position = { blockCenter.x - (transform.size.x/2) , blockPos.y - 300 - 1 };
			rend.texture = &textMn.Load("Effects/Lightning_Strike_Vert");
			break;
		case(3): //Down Lightning
			transform.size = { BLOCK_HEIGHT * .5f, 300 };
			transform.position = { blockCenter.x - (transform.size.x / 2) , blockPos.y + BLOCK_HEIGHT + 1 };
			rend.texture = &textMn.Load("Effects/Lightning_Strike_Vert");
			rend.flipY = true;
			break;
		}
		rend.layer = RenderLayer::VFX;

		reg.AddComponentToEntity<Transform>(lightningEffect, transform);
		reg.AddComponentToEntity<Renderable>(lightningEffect, rend);
		reg.AddComponentToEntity<AnimationData>(lightningEffect, sf::Vector2i{ 3,1 }, 3, .05f, false);

		Entity lightningCollider = reg.CreateEntity();
		reg.AddComponentToEntity<Transform>(lightningCollider, transform);
		reg.AddComponentToEntity<Collider>(lightningCollider, ColliderType::Effects, ColliderShape::Rectangle);
		reg.AddComponentToEntity<StatusEffect>(lightningCollider, ElemInfusion::Lightning, ActiveReaction::LightningCross);
		//Assign the collider as child as the visual effect, and use deferred destroy child entity to get rid of collider after next frame
		reg.AddComponentToEntity<Child>(lightningCollider, lightningEffect);
		reg.GetEventQueue().PublishDeferred<DestroyChildEntity>(lightningEffect);
	}
}