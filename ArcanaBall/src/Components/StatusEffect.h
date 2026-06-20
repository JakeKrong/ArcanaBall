#pragma once

enum class ElemInfusion : uint8_t {
	None,
	Flame,
	Ice,
	Lightning
};

struct StatusEffect {
	StatusEffect() = default;
	StatusEffect(ElemInfusion effect, float duration = 0, bool justApplied = true) :
		element(effect),
		duration(duration),
		justApplied(justApplied)
	{}

	float duration;
	ElemInfusion element{ ElemInfusion::None };
	bool justApplied = true;
};