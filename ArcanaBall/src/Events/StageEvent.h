#pragma once
#include "BaseEvent.h"

//Anything that requires reflection on the stage itself
struct StageEvent : BaseEvent {
	enum class EventType {
		BallPaddleCollision,
		ReactionTriggered
	};

	StageEvent(EventType type) :
		eventType(type)
	{}

	EventType eventType{};
};