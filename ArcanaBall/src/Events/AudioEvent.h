#pragma once
#include "BaseEvent.h"
#include "AudioAsset.h"

struct AudioEvent : BaseEvent {

	AudioEvent(AudioAsset asset, bool loop = false) :
		asset(asset),
		loop(loop)
	{}

	AudioAsset asset;
	bool loop;
};