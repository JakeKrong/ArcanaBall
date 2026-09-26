#include "PhysicsSystem.h"

#include "Registry.h"
#include "PhysicsEvent.h"

void PhysicsSystem::Step(float stepTime) {
	if (!m_Registry || m_Entities.size() == 0) {
		return;
	}

	auto& registry = *m_Registry;
	auto& physicsCompArr = registry.GetComponentArray<Physics>();
	auto& transformCompArr = registry.GetComponentArray<Transform>();
	
	for (const PhysicsEvent& event : registry.GetEventQueue().ConsumeTEvents<PhysicsEvent>()) {
		if (!registry.EntityHasComponent<Physics>(event.ent)) continue;

		Physics& physComp = physicsCompArr.GetTComponent(event.ent);
		physComp.velocity += event.vectorChange;
		if (event.inverseX) physComp.velocity.x *= -1;
		if (event.inverseY) physComp.velocity.y *= -1;
		if (event.setAngle != -1) physComp.velocity = sf::Vector2f{ physComp.velocity.length(), sf::Angle(sf::degrees(event.setAngle)) };
	}

	for (Entity ent : m_Entities) {
		if (!m_Registry->IsEntityLive(ent)) continue;

		Transform& transComp = transformCompArr.GetTComponent(ent);
		Physics& physComp = physicsCompArr.GetTComponent(ent);

		transComp.position += physComp.velocity * stepTime;
	}
}