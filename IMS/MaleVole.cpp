#include "MaleVole.h"
#include <iostream>
#include <algorithm>
#include "FemaleVole.h"

MaleVole::MaleVole(unsigned int adultGen, int height, int width) : Vole(adultGen, height, width)
{
}

void MaleVole::NextGeneration(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned generation, int height, int width)
{
	if (WillDie)
	{
		(*field)[height][width] = nullptr;
		return;
	}
	nextGenerationHeight = height;
	nextGenerationWidth = width;
	if (adultGeneration == generation)
	{
		isAdult = true;
		return;
	}
	int maxHeight = field->size() - 1;
	int maxWidth = field[0].size() - 1;
	if (isAdult)
	{
		std::unique_ptr<Cell>* target = FindClose(CellTypes::FemaleVole, field);
		if (target == nullptr)
			target = FindFar(CellTypes::FemaleVole, field);
		if (target != nullptr)
		{
			GoToDirection(target->get()->nextGenerationHeight - nextGenerationHeight, target->get()->nextGenerationWidth - nextGenerationWidth, maxHeight, maxWidth);
			return;
		}
	}
	std::unique_ptr<Cell>* food = FindFar(CellTypes::Poison, field);
	if (food == nullptr)
		food = FindFar(CellTypes::Poison, field);
	if (food != nullptr)
	{
		GoToDirection(food->get()->nextGenerationHeight - nextGenerationHeight, food->get()->nextGenerationWidth - nextGenerationWidth, maxHeight, maxWidth);
		return;
	}
	GoToRandomDirection(maxHeight, maxWidth);

}

CellTypes MaleVole::WhatAmI()
{
	return CellTypes::MaleVole;
}

std::unique_ptr<Cell>* MaleVole::FindCloseForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	return FindForReproduction(field, 1);
}

std::unique_ptr<Cell>* MaleVole::FindFarForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	return FindForReproduction(field, 2);
}

std::unique_ptr<Cell>* MaleVole::FindForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field, int distance)
{
	const int maxHeight = (*field).size() - 1;
	const int maxWidth = (*field)[0].size() - 1;
	for (auto i = -distance; i <= distance; ++i)
	{
		const auto height = i < 0 ? std::max(nextGenerationHeight + i, 0) : std::min(nextGenerationHeight + i, maxHeight);
		for (auto j = -distance; j <= distance; ++j)
		{
			if (std::abs(i) < distance && std::abs(j) < distance)
				continue;
			const auto width = j < 0 ? std::max(nextGenerationWidth + j, 0) : std::min(nextGenerationWidth + j, maxWidth);
			const auto type = (*field)[height][width]->WhatAmI();
			if (type == CellTypes::FemaleVole)
			{
				const auto target = dynamic_cast<FemaleVole*>((*field)[height][width].get());
				if (target->isAdult && !target->isPregnant)
					return &(*field)[height][width];
			}
		}
	}
	return nullptr;
}