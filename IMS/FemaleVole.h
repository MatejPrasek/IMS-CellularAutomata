#pragma once
#include "Vole.h"
class FemaleVole final :
	public Vole
{
public:
	bool isPregnant;
	FemaleVole(unsigned adultGen, int height, int width);
	void NextGeneration(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned generation, int height, int width) override;
	CellTypes WhatAmI() override;
	void GiveBirth(std::vector<std::vector<std::unique_ptr<Cell>>>* field, Cell* collider);
	std::unique_ptr<Cell>* FindCloseForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field);
	std::unique_ptr<Cell>* FindFarForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field);
	std::unique_ptr<Cell>* FindForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field, int distance);

	static const int ChildTime = 15 * 24;
	static const int PregnantTime = 21 * 24;
	unsigned int birthGeneration;

private:
	bool givingBirth;
};

