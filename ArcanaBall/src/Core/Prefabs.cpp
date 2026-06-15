#include "Prefabs.h"
#include "Registry.h"

Entity Prefab::Button(Registry& reg, TextureManager& textMn) {
	return 1;
}

Entity Prefab::Ball(Registry& reg, TextureManager& textMn) {
	Entity ball = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(ball, sf::Vector2f{ 640,360 }, sf::Vector2f{ 20,20 });
	reg.AddComponentToEntity<Renderable>(ball, &textMn.Load("Ball"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(ball, ColliderType::Ball, ColliderShape::Circle);
	reg.AddComponentToEntity<Physics>(ball, sf::Vector2f{ 1000,1000 });

	return ball;
}

Entity Prefab::Brick(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity brick = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(brick, position, sf::Vector2f{ 30,20 });
	reg.AddComponentToEntity<Renderable>(brick, &textMn.Load("Brick"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(brick, ColliderType::Block, ColliderShape::Rectangle);

	return brick;
}

Entity Prefab::Wood(Registry& reg, TextureManager& textMn, sf::Vector2f position) {
	Entity wood = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(wood, position, sf::Vector2f{ 30,20 });
	reg.AddComponentToEntity<Renderable>(wood, &textMn.Load("Wood"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(wood, ColliderType::Block, ColliderShape::Rectangle);

	return wood;
}

void Prefab::LevelBorders(Registry& reg, TextureManager& textMn) {
	Entity topWall = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(topWall,  sf::Vector2f{ 0,0 }, sf::Vector2f{1280,50});
	reg.AddComponentToEntity<Renderable>(topWall, &textMn.Load("Level_Border"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(topWall, ColliderType::Border, ColliderShape::Rectangle);

	Entity leftWall = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(leftWall, sf::Vector2f{ 0,0 }, sf::Vector2f{ 50,720 });
	reg.AddComponentToEntity<Renderable>(leftWall, &textMn.Load("Level_Border"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(leftWall, ColliderType::Border, ColliderShape::Rectangle);

	Entity rightWall = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(rightWall, sf::Vector2f{ 1230,0 }, sf::Vector2f{ 50,720 });
	reg.AddComponentToEntity<Renderable>(rightWall, &textMn.Load("Level_Border"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(rightWall, ColliderType::Border, ColliderShape::Rectangle);

	Entity botWall = reg.CreateEntity();
	reg.AddComponentToEntity<Transform>(botWall, sf::Vector2f{ 0,670 }, sf::Vector2f{ 1280,50 });
	reg.AddComponentToEntity<Renderable>(botWall, &textMn.Load("Level_Border"), RenderLayer::GameObjects);
	reg.AddComponentToEntity<Collider>(botWall, ColliderType::Border, ColliderShape::Rectangle);
}