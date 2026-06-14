#pragma once

#include "Types.h"
#include "Registry.h"
#include "TextureManager.h"

namespace Prefab{
	Entity Button(Registry&, TextureManager&);
	Entity Ball(Registry&, TextureManager&);
	void LevelBorders(Registry&, TextureManager&);
};