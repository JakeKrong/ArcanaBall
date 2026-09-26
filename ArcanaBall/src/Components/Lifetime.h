#pragma once

//A bounded existence, counted in collision passes rather than seconds so it stays exact
//at any framerate. LifetimeSystem runs immediately after CollisionSystem, so the count
//reads literally: 1 means "take part in one collision pass, then go".
struct Lifetime {
	Lifetime() = default;

	//Not explicit: ComponentArray::AddTComponent copy-list-initialises the slot
	Lifetime(int passes) :
		collisionPasses(passes)
	{}

	int collisionPasses{ 1 };
};
