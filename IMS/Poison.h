#pragma once
#include "Cell.h"
class Poison final :
	public Cell
{
public:
	Poison(int height, int width);
	void NextGeneration(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned generation, int height, int width) override;
	CellTypes WhatAmI() override;
	void ResolveCollision(std::unique_ptr<Cell>* colliderPtr, std::vector<std::vector<std::unique_ptr<Cell>>>* field) override;
};

