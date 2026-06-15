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
	Right
};

struct ColliderBody {
	Collider& collider;
	Transform& transform;
};

struct AABB {
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
	bool HasCollision(const ColliderBody&, const ColliderBody&, HitFromDir* = nullptr);

	void Update();

private:

	//Temp const (to be adjusted)
	static constexpr int COLS = 15, ROWS = 10;
	static constexpr float BLOCK_WIDTH = 30, BLOCK_HEIGHT = 20;
	static constexpr float GRID_OFFSET_X = 400, GRID_OFFSET_Y = 300;

	sf::Vector2i StageToGrid(sf::Vector2f position) const {
		return {
			static_cast<int>((position.x - GRID_OFFSET_X) / BLOCK_WIDTH),
			static_cast<int>((position.y - GRID_OFFSET_Y) / BLOCK_HEIGHT)
		};
	}

	Entity m_BlockGrid[COLS][ROWS] = { 0 };

	std::unordered_map <std::pair<ColliderType, ColliderType>, std::function<void(Entity, Entity, HitFromDir)>, PairHashing> m_CollisionHandlerMap;
};