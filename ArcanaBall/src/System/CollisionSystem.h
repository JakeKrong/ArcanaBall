#pragma once
#include "ISystem.h"

#include <unordered_map>
#include <array>
#include <functional>

#include "Collider.h"
#include "Transform.h"

enum class HitFromDir : uint8_t {
	None,
	Top,
	Bottom,
	Left,
	Right,
	Corner
};

struct ColliderBody {
	Collider& collider;
	Transform& transform;
};

struct CollisionDetails {
	HitFromDir hitDir;
	sf::Vector2f pointOfContact{ 0,0 };
};

struct RectAABB {
	sf::Vector2f position;
	sf::Vector2f size;

	float left()   const { return position.x; }
	float right()  const { return position.x + size.x; }
	float top()    const { return position.y; }
	float bottom() const { return position.y + size.y; }
};

struct PairHashing {
	size_t operator()(const std::pair<ColliderType, ColliderType>& pair) const noexcept {
		return (static_cast<size_t>(pair.first) << 8) | static_cast<size_t>(pair.second);
	}
};


class CollisionSystem : public ISystem {
public:

	void InitBlockGridMap();
	void RegisterCollisionHandlers();
	std::array<Entity, 4> GetNearbyBlocks(const Transform&);
	bool HasCollision(const ColliderBody&, const ColliderBody&, CollisionDetails* = nullptr);

	void Update();

private:

	sf::Vector2i StageToGrid(sf::Vector2f position) const {
		return {
			static_cast<int>((position.x - GRID_OFFSET_X) / BLOCK_WIDTH),
			static_cast<int>((position.y - GRID_OFFSET_Y) / BLOCK_HEIGHT)
		};
	};

	std::array<std::array<Entity, BLOCK_ROWS>, BLOCK_COLUMNS> m_BlockGrid{};

	std::unordered_map <std::pair<ColliderType, ColliderType>, std::function<void(Entity, Entity, CollisionDetails)>, PairHashing> m_CollisionHandlerMap;
};