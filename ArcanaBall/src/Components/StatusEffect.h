#pragma once

enum class ElemInfusion : uint8_t {
	None,
	Fire,
	Ice,
	Lightning
};

enum class ActiveReaction : uint8_t {
	None,
	IceShatter,
	Overload,
	LightningCross
};

struct StatusEffect {
	StatusEffect() = default;

	StatusEffect(ElemInfusion effect, ActiveReaction reaction = ActiveReaction::None, float duration = 0, bool justApplied = true) :
		element(effect),
		reaction(reaction),
		duration(duration),
		justApplied(justApplied)
	{}

	float duration{ 0.f };
	ElemInfusion element{ ElemInfusion::None };
	ActiveReaction reaction{ ActiveReaction::None };
	bool justApplied = true;
};