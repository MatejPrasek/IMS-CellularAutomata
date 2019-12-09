#pragma once
#include "Vole.h"
class MaleVole final :
	public Vole
{
public:
	MaleVole(unsigned adultGen, int height, int width);
	void NextGeneration(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned generation, int height, int width) override;
	CellTypes WhatAmI() override;
	std::unique_ptr<Cell>* FindCloseForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field);
	std::unique_ptr<Cell>* FindFarForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field);
	std::unique_ptr<Cell>* FindForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field, int distance);

	static const int ChildTime = 42 * 24;
};

