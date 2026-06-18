#include "Prefabs.h"
#include "Registry.h"

Entity Prefab::UI::Button(Registry& reg, TextureManager& textMn) {
	return 1;
}

Entity Prefab::GameObject::Ball(Registry& reg, TextureManager& textMn) {
	Entity ball = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(ball, sf::Vector2f{ 640,560 }, sf::Vector2f{ 20,20 });
	reg.AddComponentToEntity<Renderable>(ball, &textMn.Load("GameObject/Ball"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(ball, ColliderType::Ball, ColliderShape::Circle);
	reg.AddComponentToEntity<Physics>(ball, sf::Vector2f{ 600,600 });
	reg.AddComponentToEntity<StatusEffect>(ball, ElemInfusion::None);

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
	reg.AddComponentToEntity<Block>(wood, BlockType::Brick, 2.f);

	return wood;
}

Entity Prefab::GameObject::Steel(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity steel = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(steel, position, sf::Vector2f{ BLOCK_WIDTH,BLOCK_HEIGHT });
	reg.AddComponentToEntity<Renderable>(steel, &textMn.Load("GameObject/Steel"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(steel, ColliderType::Block, ColliderShape::Rectangle);
	reg.AddComponentToEntity<Block>(steel, BlockType::Brick, 10.f);

	return steel;
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
	reg.AddComponentToEntity<Transform>(killZone, sf::Vector2f{250,720}, sf::Vector2f{ 755, 200 });
	reg.AddComponentToEntity<Collider>(killZone, ColliderType::OutZone, ColliderShape::Rectangle);

	return killZone;
}