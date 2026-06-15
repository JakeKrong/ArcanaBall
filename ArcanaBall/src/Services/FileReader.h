#pragma once
#include <unordered_map>
#include <array>
#include <string>

#include "Types.h"

class FileReader {
public:
	static bool ReadLevelData(std::unordered_map<int, StageGridData>&);
};