#pragma once
#include <cstdint>
#include <bitset>
#include <memory>
#include <array>

// *** Game Configurations *** //
#define TargetFixedUpdateFreq 144
#define DefaultVolumeSetting 20
#define DefaultResolution {1280, 720}


// *** ECS Functionality *** //
using Entity = std::uint8_t;
using ComponentID = std::uint16_t;

const Entity ENTITY_CAP = 255;			//Expected upper limit for entity count
const ComponentID COMPONENT_CAP = 16;	//Cap for component types

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