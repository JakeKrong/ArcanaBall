#pragma once
#include "ISystem.h"

#include <unordered_map>
#include <functional>
#include <optional>

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


class CollisionSystem : public ISystem{
public:

	void InitBlockGridMap();
	void UpdateLastblockInd();
	void RegisterCollisionHandlers();

	bool HasCollision(const ColliderBody&, const ColliderBody&, HitFromDir* = nullptr);

	void Update();

private:

	int lastBlockEntIndex = -1, prevEntityCnt = 0, prevLastEntity = 0;

	//Temp const
	static constexpr int cols = 15, rows = 20;
	Entity m_BlockGrid[cols][rows] = { 0 };

	std::unordered_map <std::pair<ColliderType, ColliderType>, std::function<void(Entity, Entity, HitFromDir)>, PairHashing> m_CollisionHandlerMap;
};