#pragma once
#include "CellTypes.h"
#include <memory>
#include <vector>

class Cell
{
private:

public:
	virtual ~Cell() = default;
	virtual void NextGeneration(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned int generation, int height, int width) = 0;
	virtual void ResolveCollision(std::unique_ptr<Cell>* colliderPtr, std::vector<std::vector<std::unique_ptr<Cell>>>* field) = 0;
	virtual CellTypes WhatAmI() = 0;

	int nextGenerationHeight;
	int nextGenerationWidth;
};

