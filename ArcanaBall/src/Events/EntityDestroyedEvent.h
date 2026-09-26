#pragma once
#include "BaseEvent.h"
#include "Types.h"

struct DestroyChildEntity : BaseEvent {
	DestroyChildEntity(Entity ent) :
		parentEntity(ent)
	{}
	Entity parentEntity{ 0 };
};