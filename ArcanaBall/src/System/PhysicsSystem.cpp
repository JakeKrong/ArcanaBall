#include "PhysicsSystem.h"

#include "Registry.h"
#include "PhysicsEvent.h"

#define FixedUpdateStep static_cast<float>(1.0/TargetFixedUpdateFreq)

void PhysicsSystem::Update(float deltaTime) {
	if (!m_Registry || m_Entities.size() == 0) {
		return;
	}

	auto& registry = *m_Registry;
	auto& physicsCompArr = registry.GetComponentArray<Physics>();
	auto& transformCompArr = registry.GetComponentArray<Transform>();
	
	for (const auto& event : registry.GetEventQueue().GetTEvents<PhysicsEvent>()) {
		Physics& physComp = physicsCompArr.GetTComponent(event->ent);
		physComp.velocity += event->vectorChange;
		if (event->inverseX) physComp.velocity.x *= -1;
		if (event->inverseY) physComp.velocity.y *= -1;
		if (event->nudgeWithDelta) transformCompArr.GetTComponent(event->ent).position += physComp.velocity * (deltaTime/5.f);
	}

	timeSinceFixedUpdate += deltaTime;

	if (timeSinceFixedUpdate >= FixedUpdateStep) {
		timeSinceFixedUpdate -= FixedUpdateStep;

		for (Entity ent : m_Entities) {
			Transform& transComp = transformCompArr.GetTComponent(ent);
			Physics& physComp = physicsCompArr.GetTComponent(ent);

			transComp.position += physComp.velocity * deltaTime;
		}
	}
}