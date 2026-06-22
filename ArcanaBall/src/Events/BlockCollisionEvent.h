#pragma once
#include "BaseEvent.h"
#include "Types.h"
#include "StatusEffect.h"

struct BlockCollisionEvent : BaseEvent {
	Entity blockEntity;
	ElemInfusion infusedElem = ElemInfusion::None;
	ActiveReaction reaction = ActiveReaction::None;
};