#pragma once
#include "BaseEvent.h"
#include "Types.h"

struct ChangeTextureEvent : BaseEvent {
	ChangeTextureEvent(Entity ent, const std::string& textureName) :
		entity(ent),
		newTextureName(textureName)
	{ }

	Entity entity;
	std::string newTextureName;
};