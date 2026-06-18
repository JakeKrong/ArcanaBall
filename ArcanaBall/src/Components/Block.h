#pragma once

enum class BlockType {
	Stone,
	Wood,
	Brick,
	Steel
};

struct Block {
	BlockType blockType;
	float durabilty = 1;

	//bool isMarkedForDestruction = false;
};