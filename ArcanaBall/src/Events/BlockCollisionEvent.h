#pragma once
#include "BaseEvent.h"
#include "Types.h"
#include "StatusEffect.h"

struct BlockCollisionEvent : BaseEvent {
	enum class CollisionSource {
		Ball,
		Reactions
	};

	Entity blockEntity;
	StatusEffect colliderInfusedElement = ElemInfusion::None;
};