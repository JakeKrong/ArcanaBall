#include "CollisionSystem.h"

#include <algorithm>
#include <cassert>

#include "Registry.h"
#include "EventQueue.h"

#include "PhysicsEvent.h"
#include "BlockCollisionEvent.h"
#include "EntityDestroyedEvent.h"
#include "GameStateEvent.h"
#include "StageEvent.h"

//TESTING
#include <iostream>


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

		assert(posGridMapping.x >= 0 && posGridMapping.x < BLOCK_COLUMNS);
		assert(posGridMapping.y >= 0 && posGridMapping.y < BLOCK_ROWS);

		m_BlockGrid[posGridMapping.x][posGridMapping.y] = ent;
	}
}

void CollisionSystem::RegisterCollisionHandlers() {
	Registry& regRef = *m_Registry;
	m_CollisionHandlerMap.emplace(std::pair{ ColliderType::Ball, ColliderType::Block }, [registry = &regRef](Entity ball, Entity block, CollisionDetails colDet)
		{
			PhysicsEvent physEvent;
			physEvent.ent = ball;
			Transform& ballTrans = registry->GetEntityComponent<Transform>(ball);

			switch (colDet.hitDir) {
			case(HitFromDir::Left):
				physEvent.inverseX = true;
				ballTrans.position.x = colDet.pointOfContact.x - ballTrans.size.x;
				break;
			case(HitFromDir::Right):
				physEvent.inverseX = true;
				ballTrans.position.x = colDet.pointOfContact.x;
				break;
			case(HitFromDir::Top):
				physEvent.inverseY = true;
				ballTrans.position.y = colDet.pointOfContact.y - ballTrans.size.y;
				break;
			case(HitFromDir::Bottom):
				physEvent.inverseY = true;
				ballTrans.position.y = colDet.pointOfContact.y;
				break;
			case(HitFromDir::Corner):
				Transform& blockTrans = registry->GetEntityComponent<Transform>(block);
				RectAABB blockAABB = { blockTrans.position, blockTrans.size } ;

				//Decide angle of deflection based on which corner of the block was hit
				if (colDet.pointOfContact == sf::Vector2f{ blockAABB.left(), blockAABB.top() }) physEvent.setAngle = 225;
				else if (colDet.pointOfContact == sf::Vector2f{ blockAABB.right(), blockAABB.top() }) physEvent.setAngle = 315;
				else if (colDet.pointOfContact == sf::Vector2f{ blockAABB.right(), blockAABB.bottom() }) physEvent.setAngle = 45;
				else if (colDet.pointOfContact == sf::Vector2f{ blockAABB.left(), blockAABB.bottom() }) physEvent.setAngle = 135;
				else {
					//Worse case, just flip the velocity of the ball
					physEvent.inverseX = true;
					physEvent.inverseY = true;
				}
				break;
			}
			registry->GetEventQueue().Publish<PhysicsEvent>(physEvent);

			BlockCollisionEvent blockColEvent;
			blockColEvent.blockEntity = block;
			blockColEvent.infusedElem = registry->GetEntityComponent<StatusEffect>(ball).element;
			registry->GetEventQueue().Publish<BlockCollisionEvent>(blockColEvent);
		}
	);
	m_CollisionHandlerMap.emplace(std::pair{ ColliderType::Ball, ColliderType::Paddle }, [registry = &regRef](Entity ball, Entity paddle, CollisionDetails colDet)
		{
			//Bounce ball if top collision detected, angle based on part of paddle hit
			if (colDet.hitDir == HitFromDir::Top || colDet.hitDir == HitFromDir::Corner) {
				PhysicsEvent event;
				event.ent = ball;
				Transform& ballTrans = registry->GetEntityComponent<Transform>(ball);
				Transform& paddleTrans = registry->GetEntityComponent<Transform>(paddle);

				//event.inverseY = true;
				float paddleCenter = paddleTrans.position.x + paddleTrans.size.x * 0.5f;
				float halfPaddleWidth = paddleTrans.size.x * 0.5f;

				float relativeHit = (colDet.pointOfContact.x - paddleCenter) / halfPaddleWidth;
				event.setAngle = 270.0f + relativeHit * 70.0f;

				//Resolve collision overlap
				ballTrans.position.y = colDet.pointOfContact.y - ballTrans.size.y;

				registry->GetEventQueue().Publish<PhysicsEvent>(event);
				registry->GetEventQueue().Publish<StageEvent>(StageEvent::EventType::BallPaddleCollision);
			}
		}
	);
	m_CollisionHandlerMap.emplace(std::pair{ ColliderType::Ball, ColliderType::Border }, [registry = &regRef, this](Entity ball, Entity border, CollisionDetails colDet)
		{
			PhysicsEvent event;
			event.ent = ball;
			Transform& ballTrans = registry->GetEntityComponent<Transform>(ball);
			switch (colDet.hitDir) {
			case(HitFromDir::Left):
				event.inverseX = true;
				ballTrans.position.x = colDet.pointOfContact.x - ballTrans.size.x;
				break;
			case(HitFromDir::Right):
				event.inverseX = true;
				ballTrans.position.x = colDet.pointOfContact.x;
				break;
			case(HitFromDir::Top):
				event.inverseY = true;
				ballTrans.position.y = colDet.pointOfContact.y - ballTrans.size.y;
				break;
			case(HitFromDir::Bottom):
				event.inverseY = true;
				ballTrans.position.y = colDet.pointOfContact.y;
				break;
			}
			registry->GetEventQueue().Publish<PhysicsEvent>(event);
		}
	);
	m_CollisionHandlerMap.emplace(std::pair{ ColliderType::Ball, ColliderType::OutZone }, [registry = &regRef](Entity ball, Entity outZone, CollisionDetails colDet)
		{
			registry->GetEventQueue().Publish<GameStateEvent>({ GameStateEvent::Type::GameOver });
			registry->GetEntityComponent<Transform>(ball).position.y = 1000;
		}
	);
	m_CollisionHandlerMap.emplace(std::pair{ ColliderType::Effects, ColliderType::Block }, [registry = &regRef](Entity effect, Entity block, CollisionDetails colDet)
		{
			BlockCollisionEvent blockColEvent;
			blockColEvent.blockEntity = block;
			StatusEffect& effectStatus = registry->GetEntityComponent<StatusEffect>(effect);
			blockColEvent.infusedElem = effectStatus.element;
			blockColEvent.reaction = effectStatus.reaction;
			registry->GetEventQueue().Publish<BlockCollisionEvent>(blockColEvent);
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
	int endX = std::min(BLOCK_COLUMNS - 1, maxGrid.x);
	int startY = std::max(0, minGrid.y);
	int endY = std::min(BLOCK_ROWS - 1, maxGrid.y);

	for (int y = startY; y <= endY; ++y) {
		for (int x = startX; x <= endX; ++x) {	
			if (m_BlockGrid[x][y] != 0 && cnt < 4) {
				candidates[cnt++] = m_BlockGrid[x][y];
			}
		}
	}
	return candidates;
}

bool CollisionSystem::HasCollision(const ColliderBody& a, const ColliderBody& b, CollisionDetails* colDet) {
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
		RectAABB rectAABB(b.transform.position, b.transform.size);

		float closestX = std::clamp(circleCenter.x, rectAABB.left(), rectAABB.right());
		float closestY = std::clamp(circleCenter.y, rectAABB.top(), rectAABB.bottom());

		float dx = circleCenter.x - closestX;
		float dy = circleCenter.y - closestY;
		hasHit = (dx * dx + dy * dy) <= (circleRadius * circleRadius);

		if (hasHit && colDet) {
			(*colDet).pointOfContact = { closestX, closestY };

			float distLeft = std::abs(closestX - rectAABB.left());
			float distRight = std::abs(closestX - rectAABB.right());
			float distTop = std::abs(closestY - rectAABB.top());
			float distBottom = std::abs(closestY - rectAABB.bottom());

			//Get Hit Direction and Collision Point
			if (std::min(distTop, distBottom) == std::min(distLeft, distRight)) {
				(*colDet).hitDir = HitFromDir::Corner;
			}
			else if (std::min(distTop, distBottom) <= std::min(distLeft, distRight)) {
				if (distTop < distBottom) {
					(*colDet).hitDir = HitFromDir::Top;
				}
				else {
					(*colDet).hitDir = HitFromDir::Bottom;
				}
			}
			else {
				if (distLeft < distRight) {
					(*colDet).hitDir = HitFromDir::Left;
				}
				else {
					(*colDet).hitDir = HitFromDir::Right;
				}
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

	//Update m_BlockGrid when BlockDestroyed event received
	for (auto event : m_Registry->GetEventQueue().GetTEvents<BlockDestroyed>()) {
		sf::Vector2i destroyedBlockGrid = StageToGrid(event->blockPos);
		if (destroyedBlockGrid.x >= 0 && destroyedBlockGrid.x < BLOCK_COLUMNS &&
			destroyedBlockGrid.y >= 0 && destroyedBlockGrid.y < BLOCK_ROWS) {
			m_BlockGrid[destroyedBlockGrid.x][destroyedBlockGrid.y] = 0;
		}
	}

	for (int i = 0; i < m_Entities.size(); i++) {
		Entity ent = m_Entities[i];
		ColliderType colType = colCompArr.GetTComponent(ent).colType;

		//Only collision check needed is Ball on everthing, and effects on blocks
		switch (colType) {
		case(ColliderType::Ball): 
		{
			const auto& nearbyBlocks = GetNearbyBlocks(transCompArr.GetTComponent(ent));

			int activeBlockCount = std::ranges::count_if(nearbyBlocks, [](Entity block) { return block != 0; });

			if (activeBlockCount > 0) {
				if (activeBlockCount == 1) {
					Entity block = *std::ranges::find_if(nearbyBlocks, [](Entity b) { return b != 0; });

					const ColliderBody ballColBody{ colCompArr.GetTComponent(ent), transCompArr.GetTComponent(ent) };
					const ColliderBody otherColBody{ colCompArr.GetTComponent(block), transCompArr.GetTComponent(block) };
					CollisionDetails colDet;

					if (HasCollision(ballColBody, otherColBody, &colDet)) {
						m_CollisionHandlerMap.at(std::pair{ ColliderType::Ball, ColliderType::Block })(ent, block, colDet);
					}
				}
				else {
					//std::cout << "Block count: " << activeBlockCount << "\n";
					Entity closestBlock = 0;
					CollisionDetails closestHitDetails;
					float closestDistSq = std::numeric_limits<float>::max();

					const auto& ballTrans = transCompArr.GetTComponent(ent);
					float radius = ballTrans.size.x / 2.0f;
					sf::Vector2f ballCenter = ballTrans.position + sf::Vector2f{ radius, radius };

					for (Entity block : nearbyBlocks) {
						if (block == 0) continue;

						const ColliderBody ballColBody{ colCompArr.GetTComponent(ent), transCompArr.GetTComponent(ent) };
						const ColliderBody otherColBody{ colCompArr.GetTComponent(block), transCompArr.GetTComponent(block) };
						CollisionDetails colDet;

						if (HasCollision(ballColBody, otherColBody, &colDet)) {
							const auto& blockTrans = transCompArr.GetTComponent(block);
							sf::Vector2f blockCenter = blockTrans.position + (blockTrans.size / 2.0f);

							float dx = ballCenter.x - blockCenter.x;
							float dy = ballCenter.y - blockCenter.y;
							float distSq = (dx * dx) + (dy * dy);

							if (distSq < closestDistSq) {
								closestDistSq = distSq;
								closestBlock = block;
								closestHitDetails = colDet;
							}
						}
					}

					if (closestBlock != 0) {
						m_CollisionHandlerMap.at(std::pair{ ColliderType::Ball, ColliderType::Block })(ent, closestBlock, closestHitDetails);
					}
				}
			}

			//Iterate over rest of the vector to handle each collision
			for (int j = i + 1; j < m_Entities.size(); j++) {
				Entity otherEnt = m_Entities[j];
				auto otherColType = colCompArr.GetTComponent(otherEnt).colType;
				if (otherColType == ColliderType::Block) continue;

				if (m_CollisionHandlerMap.contains(std::pair{ ColliderType::Ball, otherColType })) {
					const ColliderBody ballColBody{ colCompArr.GetTComponent(ent), transCompArr.GetTComponent(ent) };
					const ColliderBody otherColBody{ colCompArr.GetTComponent(otherEnt), transCompArr.GetTComponent(otherEnt) };
					CollisionDetails colDet;
					if (HasCollision(ballColBody, otherColBody, &colDet)) {
						m_CollisionHandlerMap.at(std::pair{ ColliderType::Ball, otherColType })(ent, otherEnt, colDet);
					}
				}
			}
			break;
		}
		case(ColliderType::Effects):
			const auto& effectTrans = transCompArr.GetTComponent(ent);
			sf::Vector2i minGrid = StageToGrid(effectTrans.position);
			sf::Vector2i maxGrid = StageToGrid(effectTrans.position + effectTrans.size);

			int startX = std::max(0, minGrid.x);
			int endX = std::min(BLOCK_COLUMNS - 1, maxGrid.x);
			int startY = std::max(0, minGrid.y);
			int endY = std::min(BLOCK_ROWS - 1, maxGrid.y);

			for (int y = startY; y <= endY; ++y) {
				for (int x = startX; x <= endX; ++x) {
					Entity blockEnt = m_BlockGrid[x][y];
					if (blockEnt == 0) continue; // Empty grid, skip

					const ColliderBody effectColBody{ colCompArr.GetTComponent(ent), transCompArr.GetTComponent(ent) };
					const ColliderBody blockColBody{ colCompArr.GetTComponent(blockEnt), transCompArr.GetTComponent(blockEnt) };
					CollisionDetails colDet;

					if (HasCollision(effectColBody, blockColBody, &colDet)) {
						auto handlerKey = std::pair{ ColliderType::Effects, ColliderType::Block };

						if (m_CollisionHandlerMap.contains(handlerKey)) {
							m_CollisionHandlerMap.at(handlerKey)(ent, blockEnt, colDet);
						}
					}
				}
			}
			break;
		}
	}
}