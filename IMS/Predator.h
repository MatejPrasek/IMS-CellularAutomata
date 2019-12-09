#pragma once
#include "Cell.h"
class Predator final :
	public Cell
{
public:
	Predator(int height, int width);
	void NextGeneration(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned generation, int height, int width) override;
	CellTypes WhatAmI() override;
	void ResolveCollision(std::unique_ptr<Cell>* colliderPtr, std::vector<std::vector<std::unique_ptr<Cell>>>* field) override;

private:
	void GetDirection(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned generation);
	std::unique_ptr<Cell>* GetCellToPlace(std::vector<std::vector<std::unique_ptr<Cell>>>* field, Cell* collider);
	unsigned int fedUntilGeneration;

	const int fedDuration = 3;
};

