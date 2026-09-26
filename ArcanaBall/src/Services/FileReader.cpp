#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <print>

static std::string m_GameDataBasePath = "assets/";

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

    // Commit the buffered grid, but only if the level supplied every row
    auto commitLevel = [&]() {
        if (currentLevel == -1) return;

        if (currentRow == BLOCK_ROWS) m_LevelData[currentLevel] = tempGrid;
        else std::println("[Level Reader] Warning: Level {} has {} rows, expected {}. Skipped.",
            currentLevel, currentRow, BLOCK_ROWS);
    };

    while (std::getline(file, line)) {
        // Drop the CR of a CRLF file read on a platform that does not strip it
        if (!line.empty() && line.back() == '\r') line.pop_back();

        // Skip empty lines or if is comment
        if (line.empty() || line[0] == '#') continue;

        // Detect new level boundary character ("[Level 1]")
        if (line[0] == '[' && line.back() == ']') {
            // Save the last level data (if valid)
            commitLevel();

            // Extract level identifier number cleanly. Collecting the digits rather than
            // slicing at a fixed offset keeps a malformed header from throwing.
            std::string numStr;
            for (char ch : line) {
                if (ch >= '0' && ch <= '9') numStr += ch;
            }

            if (numStr.empty() || numStr.size() > 9) {
                std::println("[Level Reader] Warning: Skipping malformed level header: {}", line);
                currentLevel = -1;
                continue;
            }

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
    commitLevel();

    if (m_LevelData.empty()) {
        std::println("[Level Reader] Error: LevelData.txt contained no usable levels!");
        return false;
    }

    return true;
}