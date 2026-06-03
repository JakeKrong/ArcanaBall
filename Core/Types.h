#pragma once
#include <cstdint>
#include <bitset>
#include <memory>

#define TargetFixedUpdateFreq 144
#define DefaultVolumeSetting 20

using Entity = std::uint8_t;
using ComponentID = std::uint16_t;

const Entity ENTITY_CAP = 300;			//Expected upper limit for entity count
const ComponentID COMPONENT_CAP = 16;	//Cap for components

using Signature = std::bitset<COMPONENT_CAP>;

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
Scope<T> CreateScope(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
Scope<T> CreateRef(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}
