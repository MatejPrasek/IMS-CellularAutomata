#include "FemaleVole.h"
#include <iostream>
#include "MaleVole.h"
#include <algorithm>

FemaleVole::FemaleVole(unsigned int adultGen, int height, int width) : Vole(adultGen, height, width)
{
	birthGeneration = 0;
	givingBirth = false;
	isPregnant = false;
}

void FemaleVole::NextGeneration(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned generation, int height, int width)
{
	if (WillDie)
	{
		(*field)[height][width] = nullptr;
		return;
	}
	nextGenerationHeight = height;
	nextGenerationWidth = width;
	if(birthGeneration == generation)
	{
		givingBirth = true;
		return;
	}
	if(adultGeneration == generation)
	{
		isAdult = true;
		return;
	}
	int maxHeight = field->size() - 1;
	int maxWidth = field[0].size() - 1;
	if(isAdult && !isPregnant)
	{
		if (FindClose(CellTypes::MaleVole, field) != nullptr) 
		{
			isPregnant = true;
			birthGeneration = generation + PregnantTime;
			return;
		}
		std::unique_ptr<Cell>* target = FindFar(CellTypes::MaleVole, field);
		if(target != nullptr)
		{
			GoToDirection(target->get()->nextGenerationHeight - nextGenerationHeight, target->get()->nextGenerationWidth - nextGenerationWidth, maxHeight, maxWidth);
			return;
		}
	}
	std::unique_ptr<Cell>* food = FindFar(CellTypes::Poison, field);
	if (food == nullptr)
		food = FindFar(CellTypes::Poison, field);
	if(food != nullptr)
	{
		GoToDirection(food->get()->nextGenerationHeight - nextGenerationHeight, food->get()->nextGenerationWidth - nextGenerationWidth, maxHeight, maxWidth);
		return;
	}
	GoToRandomDirection(maxHeight,maxWidth);
}

CellTypes FemaleVole::WhatAmI()
{
	return CellTypes::FemaleVole;
}

void FemaleVole::GiveBirth(std::vector<std::vector<std::unique_ptr<Cell>>>* field, Cell* collider)
{
	if(!givingBirth)
		return;
	givingBirth = false;
	isPregnant = false;
	std::unique_ptr<Cell>* place = nullptr;
	int iterator = 0;
	const auto numberOfChild = rand() % 6 + 3;
	for (auto i = 0; i < numberOfChild; ++i)
	{
		while (place == nullptr && iterator < 3)
		{
			place = Find(CellTypes::Blank, field, iterator++);
		}

		if(place == nullptr)
			return;

		if(rand() % 2 == 0)
		{
			*place = std::make_unique<MaleVole>(birthGeneration + MaleVole::ChildTime, place->get()->nextGenerationHeight, place->get()->nextGenerationWidth);
		}
		else
		{
			*place = std::make_unique<FemaleVole>(birthGeneration + FemaleVole::ChildTime, place->get()->nextGenerationHeight, place->get()->nextGenerationWidth);
		}
		place = nullptr;
	}
}

std::unique_ptr<Cell>* FemaleVole::FindCloseForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	return FindForReproduction(field, 1);
}

std::unique_ptr<Cell>* FemaleVole::FindFarForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	return FindForReproduction(field, 2);
}

std::unique_ptr<Cell>* FemaleVole::FindForReproduction(std::vector<std::vector<std::unique_ptr<Cell>>>* field, int distance)
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
			if (type == CellTypes::MaleVole)
			{
				if(dynamic_cast<MaleVole*>((*field)[height][width].get())->isAdult)
					return &(*field)[height][width];
			}
		}
	}
	return nullptr;
}
