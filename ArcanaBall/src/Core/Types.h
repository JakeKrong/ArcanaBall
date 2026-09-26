#pragma once
#include <cstdint>
#include <bitset>
#include <memory>
#include <array>

#include <SFML/System/Vector2.hpp>

// *** Game Configurations *** //
constexpr unsigned int TargetFixedUpdateFreq = 144;
constexpr float FixedUpdateStep = 1.f / TargetFixedUpdateFreq;
constexpr sf::Vector2u DefaultResolution{ 1280, 720 };

// Tracker for volume slider, referenced by the prefab that lays the slider out and the system that drags it
constexpr float VolumeSliderMinX = 1035.f;
constexpr float VolumeSliderMaxX = 1185.f;
constexpr float DefaultVolumeSetting = 20.f;
constexpr float MaxVolumeSetting = DefaultVolumeSetting * 2.f; //Slider midpoint sits at the default


// *** ECS Functionality *** //
using Entity = std::uint16_t;
using ComponentID = std::uint16_t;

constexpr Entity ENTITY_CAP = 1000;			//Expected upper limit for entity count
constexpr ComponentID COMPONENT_CAP = 16;	//Cap for component types

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

// --- Gameplay Data --- //
constexpr static int BLOCK_ROWS = 8, BLOCK_COLUMNS = 10;
constexpr static int BLOCK_WIDTH = 60, BLOCK_HEIGHT = 30;
constexpr static int GRID_OFFSET_X = 340, GRID_OFFSET_Y = 100;

using StageGridData = std::array<std::array<std::uint8_t, BLOCK_COLUMNS>, BLOCK_ROWS>;