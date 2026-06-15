#include "CollisionSystem.h"

#include <algorithm>
#include <cassert>

#include "Registry.h"
#include "EventQueue.h"
#include "PhysicsEvent.h"

void CollisionSystem::InitBlockGridMap() {

	if (m_Entities.size() == 0) return;

	auto& transCompArr = m_Registry->GetComponentArray<Transform>();
	auto& colCompArr = m_Registry->GetComponentArray<Collider>();

	//Sort initial m_Entities based on Collider Type enum
	std::ranges::sort(m_Entities, {}, [&colCompArr](Entity ent) {
		return colCompArr.GetTComponent(ent).colType;
		});

	//Iterate sorted vector to update tracker
	for (Entity ent : m_Entities) {
		if (colCompArr.GetTComponent(ent).colType != ColliderType::Block) {
			break;
		}
		
		const auto& trans = transCompArr.GetTComponent(ent);

		auto posGridMapping = StageToGrid(trans.position);

		assert(posGridMapping.x >= 0 && posGridMapping.x < COLS);
		assert(posGridMapping.y >= 0 && posGridMapping.y < ROWS);

		m_BlockGrid[posGridMapping.x][posGridMapping.y] = ent;
	}
}

void CollisionSystem::RegisterCollisionHandlers() {
	Registry& regRef = *m_Registry;
	m_CollisionHandlerMap.emplace(std::pair{ ColliderType::Ball, ColliderType::Block }, [registry = &regRef](Entity ball, Entity block, HitFromDir hitFrom)
		{
			PhysicsEvent event;
			event.ent = ball;
			if (hitFrom == HitFromDir::Left || hitFrom == HitFromDir::Right) {
				event.inverseX = true;
			}
			else {
				event.inverseY = true;
			}
			event.nudgeWithDelta = true;
			registry->GetEventQueue().Publish<PhysicsEvent>(event);
			return;
		}
	);
	m_CollisionHandlerMap.emplace(std::pair{ ColliderType::Ball, ColliderType::Paddle }, [registry = &regRef](Entity ball, Entity paddle, HitFromDir hitFrom)
		{
			// Handle Ball-on-Paddle collision
		}
	);
	m_CollisionHandlerMap.emplace(std::pair{ ColliderType::Ball, ColliderType::Border }, [registry = &regRef, this](Entity ball, Entity border, HitFromDir hitFrom)
		{
			PhysicsEvent event;
			event.ent = ball;
			if (hitFrom == HitFromDir::Left || hitFrom == HitFromDir::Right) {
				event.inverseX = true;
			}
			else {
				event.inverseY = true;
			}
			event.nudgeWithDelta = true;
			registry->GetEventQueue().Publish<PhysicsEvent>(event);
		}
	);
	m_CollisionHandlerMap.emplace(std::pair{ ColliderType::Ball, ColliderType::OutZone }, [registry = &regRef](Entity ball, Entity outZone, HitFromDir hitFrom)
		{
			// Handle Ball-on-OutZone collision
		}
	);
	m_CollisionHandlerMap.emplace(std::pair{ ColliderType::Effects, ColliderType::Block }, [registry = &regRef](Entity effect, Entity block, HitFromDir hitFrom)
		{
			// Handle Effect-on-Block collision
		}
	);
}

std::array<Entity, 4> CollisionSystem::GetNearbyBlocks(const Transform& ballTransform) {
	std::array<Entity, 4> candidates{ 0,0,0,0 };
	int cnt = 0;

	float radius = ballTransform.size.x / 2.0f;
	sf::Vector2f center = ballTransform.position + sf::Vector2f{ radius, radius };

	sf::Vector2i minGrid = StageToGrid({ center.x - radius, center.y - radius });
	sf::Vector2i maxGrid = StageToGrid({ center.x + radius, center.y + radius });

	int startX = std::max(0, minGrid.x);
	int endX = std::min(COLS - 1, maxGrid.x);
	int startY = std::max(0, minGrid.y);
	int endY = std::min(ROWS - 1, maxGrid.y);

	for (int y = startY; y <= endY; ++y) {
		for (int x = startX; x <= endX; ++x) {	
			if (m_BlockGrid[x][y] != 0 && cnt < 4) {
				candidates[cnt++] = m_BlockGrid[x][y];
			}
		}
	}
	return candidates;
}

bool CollisionSystem::HasCollision(const ColliderBody& a, const ColliderBody& b, HitFromDir* hitDirInfo) {
	bool hasHit = false;
	//Rectangle-on-Rectangle Collision Check
	if (a.collider.colShape == ColliderShape::Rectangle && b.collider.colShape == ColliderShape::Rectangle) {
		sf::FloatRect aRect({ a.transform.position, a.transform.size });
		hasHit = aRect.findIntersection({ b.transform.position, b.transform.size }).has_value();
	}
	//Circle-on-Rectangle Collision Check
	else if (a.collider.colShape == ColliderShape::Circle && b.collider.colShape == ColliderShape::Rectangle) {
		float circleRadius = a.transform.size.x / 2;
		sf::Vector2f circleCenter = a.transform.position + sf::Vector2f{ circleRadius, circleRadius };
		AABB rectAABB(b.transform.position, b.transform.size);

		float closestX = std::clamp(circleCenter.x, rectAABB.left(), rectAABB.right());
		float closestY = std::clamp(circleCenter.y, rectAABB.top(), rectAABB.bottom());

		float dx = circleCenter.x - closestX;
		float dy = circleCenter.y - closestY;
		hasHit = (dx * dx + dy * dy) <= (circleRadius * circleRadius);

		if (hasHit && hitDirInfo) {
			float distLeft = std::abs(closestX - rectAABB.left());
			float distRight = std::abs(closestX - rectAABB.right());
			float distTop = std::abs(closestY - rectAABB.top());
			float distBottom = std::abs(closestY - rectAABB.bottom());

			//Decide later if need to factor in ball velocity in these checks
			if (std::min(distLeft, distRight) < std::min(distTop, distBottom)) {
				if (distLeft < distRight) *hitDirInfo = HitFromDir::Left;
				else *hitDirInfo = HitFromDir::Right;
			}
			//else if (std::min(distLeft, distRight) > std::min(distTop, distBottom)) {
			else {
				if (distTop < distBottom) *hitDirInfo = HitFromDir::Top;
				else *hitDirInfo = HitFromDir::Bottom;
			}
		}
	}
	return hasHit;
}


void CollisionSystem::Update() {
	if (!m_Registry || m_Entities.size() == 0) {
		return;
	}

	auto& transCompArr = m_Registry->GetComponentArray<Transform>();
	auto& colCompArr = m_Registry->GetComponentArray<Collider>();

	//Start collision check iteration after last block index
	for (int i = 0; i < m_Entities.size(); i++) {
		Entity ent = m_Entities[i];
		ColliderType colType = colCompArr.GetTComponent(ent).colType;

		//Only collision check needed is Ball on everthing, and effects on blocks
		switch (colType) {
		case(ColliderType::Ball): 
		{
			//Check against GridMap for collision -> then handle BALL on block collision
			const auto& nearbyBlocks = GetNearbyBlocks(transCompArr.GetTComponent(ent));

			for (Entity block : nearbyBlocks) {
				if (block == 0) continue;
				const ColliderBody ballColBody{ colCompArr.GetTComponent(ent), transCompArr.GetTComponent(ent) };
				const ColliderBody otherColBody{ colCompArr.GetTComponent(block), transCompArr.GetTComponent(block) };
				HitFromDir hitDir;
				if (HasCollision(ballColBody, otherColBody, &hitDir)) {
					m_CollisionHandlerMap.at(std::pair{ ColliderType::Ball, ColliderType::Block })(ent, block, hitDir);
				}
			}

			//Iterate over rest of the vector to handle each collision
			for (int j = i + 1; j < m_Entities.size(); j++) {
				Entity otherEnt = m_Entities[j];
				auto otherColType = colCompArr.GetTComponent(otherEnt).colType;

				if (m_CollisionHandlerMap.contains(std::pair{ ColliderType::Ball, otherColType })) {
					const ColliderBody ballColBody{ colCompArr.GetTComponent(ent), transCompArr.GetTComponent(ent) };
					const ColliderBody otherColBody{ colCompArr.GetTComponent(otherEnt), transCompArr.GetTComponent(otherEnt) };
					HitFromDir hitDir;
					if (HasCollision(ballColBody, otherColBody, &hitDir)) {
						m_CollisionHandlerMap.at(std::pair{ ColliderType::Ball, otherColType })(ent, otherEnt, hitDir);
					}
				}
			}
			break;
		}
		case(ColliderType::Effects):
			//Check against GridMap for collision -> then handle EFFECTS on block collision
			break;
		}
	}
}