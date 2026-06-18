#pragma once
#include "BaseEvent.h"
#include "Types.h"
#include "StatusEffect.h"

struct BlockCollisionEvent : BaseEvent {
	Entity blockEntity;
	StatusEffect colliderInfusedElement = ElemInfusion::None;
};