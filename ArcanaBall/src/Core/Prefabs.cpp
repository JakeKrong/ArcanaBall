#include "Prefabs.h"
#include "Registry.h"

std::array<Entity, 3> Prefab::UI::VolumeControl(Registry& reg, TextureManager& textMn, float currVolume) {
	Entity volumeIcon = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(volumeIcon, sf::Vector2f{ 960,25 }, sf::Vector2f{ 50,50 });
	reg.AddComponentToEntity<Renderable>(volumeIcon, &textMn.Load("UI/Volume"), RenderLayer::UI);

	Entity volumeSlider = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(volumeSlider, sf::Vector2f{ 1025,35 }, sf::Vector2f{ 180,30 });
	reg.AddComponentToEntity<Renderable>(volumeSlider, &textMn.Load("UI/Slider"), RenderLayer::UI);

	Entity sliderButton = reg.CreateEntity();
	float currSliderX = 1035.f + (currVolume / DefaultVolumeSetting * 75.f);
	reg.AddComponentToEntity<Transform>(sliderButton, sf::Vector2f{ currSliderX,35 }, sf::Vector2f{ 15,22 });
	reg.AddComponentToEntity<Renderable>(sliderButton, &textMn.Load("UI/Slider_Butt"), RenderLayer::UI);
	reg.AddComponentToEntity<Button>(sliderButton, ButtonAction::AdjustVolume);

	return { volumeIcon, volumeSlider, sliderButton };
}

Entity Prefab::UI::LifeIndicator(Registry& reg, TextureManager& textMn, int numbering) {
	Entity lifeInd = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(lifeInd, numbering == 1 ? sf::Vector2f{ 30,65 } : sf::Vector2f{ 60,65 }, sf::Vector2f{ 25, 25 });
	reg.AddComponentToEntity<Renderable>(lifeInd, &textMn.Load("GameObject/Ball"), RenderLayer::UI);
	return lifeInd;
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

Entity Prefab::GameObject::Paddle(Registry& reg, TextureManager& textMn) {
	Entity paddle = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(paddle, sf::Vector2f{ 640,600 }, sf::Vector2f{ 110,15 });
	reg.AddComponentToEntity<Renderable>(paddle, &textMn.Load("GameObject/Paddle"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(paddle, ColliderType::Paddle, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Physics>(paddle, sf::Vector2f{ 0,0 });

	return paddle;
}

// ********** BLOCKS ********** //

Entity Prefab::GameObject::Stone(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity stone = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(stone, position, sf::Vector2f{ BLOCK_WIDTH,BLOCK_HEIGHT });
	reg.AddComponentToEntity<Renderable>(stone, &textMn.Load("GameObject/Stone"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(stone, ColliderType::Block, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Block>(stone, BlockType::Stone, 1.f);

	return stone;
}

Entity Prefab::GameObject::Brick(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity brick = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(brick, position, sf::Vector2f{ BLOCK_WIDTH,BLOCK_HEIGHT });
	reg.AddComponentToEntity<Renderable>(brick, &textMn.Load("GameObject/Brick"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(brick, ColliderType::Block, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Collider>(brick, ColliderType::Block, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Block>(brick, BlockType::Brick, 2.f);

	return brick;
}

Entity Prefab::GameObject::Wood(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity wood = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(wood, position, sf::Vector2f{ BLOCK_WIDTH,BLOCK_HEIGHT });
	reg.AddComponentToEntity<Renderable>(wood, &textMn.Load("GameObject/Wood"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(wood, ColliderType::Block, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Block>(wood, BlockType::Wood, 2.f);

	return wood;
}

Entity Prefab::GameObject::Steel(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity steel = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(steel, position, sf::Vector2f{ BLOCK_WIDTH,BLOCK_HEIGHT });
	reg.AddComponentToEntity<Renderable>(steel, &textMn.Load("GameObject/Steel"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(steel, ColliderType::Block, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Block>(steel, BlockType::Steel, 10.f);

	return steel;
}

// Block breaking effects
Entity Prefab::GameObject::WoodBreak(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity woodBreak = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(woodBreak, position, sf::Vector2f{ BLOCK_WIDTH,BLOCK_HEIGHT });
	reg.AddComponentToEntity<Renderable>(woodBreak, &textMn.Load("GameObject/Wood_Break"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<AnimationData>(woodBreak, sf::Vector2i{ 2,1 }, 2, .1f, false);

	return woodBreak;
}

void Prefab::GameObject::LevelBorders(Registry& reg, TextureManager& textMn) {

	Entity leftWall = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(leftWall, sf::Vector2f{ 250,0 }, sf::Vector2f{ 25,720 });
	reg.AddComponentToEntity<Renderable>(leftWall, &textMn.Load("Level_Border"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(leftWall, ColliderType::Border, ColliderShape::Rectangle);

	Entity rightWall = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(rightWall, sf::Vector2f{ 980,0 }, sf::Vector2f{ 25,720 });
	reg.AddComponentToEntity<Renderable>(rightWall, &textMn.Load("Level_Border"), RenderLayer::GameObjects, true, true);
	reg.AddComponentToEntity<Collider>(rightWall, ColliderType::Border, ColliderShape::Rectangle);

	Entity topWall = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(topWall, sf::Vector2f{ 275,0 }, sf::Vector2f{ 705,25 });
	reg.AddComponentToEntity<Renderable>(topWall, &textMn.Load("Level_Border_Top"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(topWall, ColliderType::Border, ColliderShape::Rectangle);

	//	TEMPORARY //
	//Entity botWall = reg.CreateEntity();
	//reg.AddComponentToEntity<Transform>(botWall, sf::Vector2f{ 275,695 }, sf::Vector2f{ 705,25 });
	//reg.AddComponentToEntity<Renderable>(botWall, &textMn.Load("Level_Border_Top"), RenderLayer::GameObjects);
	//reg.AddComponentToEntity<Collider>(botWall, ColliderType::Border, ColliderShape::Rectangle);
}

Entity Prefab::GameObject::KillZone(Registry& reg) {
	Entity killZone = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(killZone, sf::Vector2f{ 250,730 }, sf::Vector2f{ 755, 25 });
	reg.AddComponentToEntity<Collider>(killZone, ColliderType::OutZone, ColliderShape::Rectangle);

	return killZone;
}