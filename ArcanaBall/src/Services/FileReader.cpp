#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <print>

#ifdef _DEBUG
static std::string m_GameDataBasePath = "../../../../ArcanaBall/assets/";
#else
static std::string m_GameDataBasePath = "assets/";
#endif

bool FileReader::ReadLevelData(std::unordered_map<int, StageGridData>& m_LevelData) {

    std::ifstream file(m_GameDataBasePath + "LevelData.txt");
    if (!file.is_open()) {
        std::println("[Level Reader] Error: Could not read  file: LevelData.txt");
        return false;
    }

    std::string line;
    int currentLevel = -1;
    int currentRow = 0;
    StageGridData tempGrid{};

    while (std::getline(file, line)) {
        // Skip empty lines or if is comment
        if (line.empty() || line[0] == '#') continue;

        // Detect new level boundary character ("[Level 1]")
        if (line[0] == '[' && line.back() == ']') {
            // Save the last level data (if valid)
            if (currentLevel != -1 && currentRow == BLOCK_ROWS) {
                m_LevelData[currentLevel] = tempGrid;
            }

            // Extract level identifier number cleanly
            std::string numStr = line.substr(7, line.size() - 8); // Strips "[Level " and "]"
            currentLevel = std::stoi(numStr);
            currentRow = 0;
            tempGrid = StageGridData{}; // Reset buffer grid
            continue;
        }

        // Parse matrix tokens inside the current level
        if (currentLevel != -1 && currentRow < BLOCK_ROWS) {
            int limit = std::min(static_cast<int>(line.size()), BLOCK_COLUMNS);

            for (int col = 0; col < limit; ++col) {
                char ch = line[col];

                if (ch >= '0' && ch <= '9') {
                    // Get raw integer value (Subtract ASCII '0' from the char)
                    tempGrid[currentRow][col] = static_cast<uint8_t>(ch - '0');
                }
            }
            currentRow++;
        }
    }

    // Flush out the absolute final layout tracking block from the loop tail
    if (currentLevel != -1 && currentRow == BLOCK_ROWS) {
        m_LevelData[currentLevel] = tempGrid;
    }

    std::println("[Level Reader] Level data loaded. Cached {} stages successfully.", m_LevelData.size());
    return true;
}