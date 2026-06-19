#pragma once
#include "BaseEvent.h"

struct MenuNavEvent : BaseEvent {
	enum class NavScreen {
		MainMenu,
		SelectLevel,
		GameGuide
	};

	MenuNavEvent(NavScreen screen) :
		screen(screen)
	{ }
	NavScreen screen;
};