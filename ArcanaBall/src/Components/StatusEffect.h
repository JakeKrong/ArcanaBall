#pragma once

enum class PassiveEffects : uint8_t {
	None,
	Flame
};

enum class ActiveEffects : uint8_t {
	None,
	Explosion
};

struct StatusEffect {
	StatusEffect() = default;
	StatusEffect(PassiveEffects effect, float duration = 0, bool justApplied = true) :
		pasEffect(effect),
		duration(duration),
		justApplied(justApplied)
	{}

	float duration;
	PassiveEffects pasEffect{ PassiveEffects::None };
	ActiveEffects actvEffect{ ActiveEffects::None };
	bool justApplied = true;
};