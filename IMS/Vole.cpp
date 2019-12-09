#include "Vole.h"
#include <algorithm>
#include "FemaleVole.h"

Vole::Vole(unsigned adultGen, int height, int width)
{
	adultGeneration = adultGen;
	nextGenerationHeight = height;
	nextGenerationWidth = width;
	isAdult = false;
	WillDie = false;
}

void Vole::ResolveCollision(std::unique_ptr<Cell>* colliderPtr, std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	auto collider = colliderPtr->get();
	std::unique_ptr<Cell>* anotherPlace = nullptr;
	int i = 1;
	switch (collider->WhatAmI())
	{
	case CellTypes::FemaleVole:
		dynamic_cast<FemaleVole*>(collider)->GiveBirth(field, collider);
	case CellTypes::MaleVole:
		while (anotherPlace == nullptr)
		{
			anotherPlace = Find(CellTypes::Blank, field, i++);
		}
		collider->nextGenerationHeight = anotherPlace->get()->nextGenerationHeight;
		collider->nextGenerationWidth = anotherPlace->get()->nextGenerationWidth;
		anotherPlace->get()->ResolveCollision(colliderPtr, field);
		break;
	case CellTypes::Predator:
		(*field)[collider->nextGenerationHeight][collider->nextGenerationWidth] = std::move(*colliderPtr);
		break;
	case CellTypes::Poison:
	case CellTypes::Blank:
	default:
		break;
	}
}

std::unique_ptr<Cell>* Vole::FindClose(CellTypes desiredType, std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	return Find(desiredType, field, 1);
}

std::unique_ptr<Cell>* Vole::FindFar(CellTypes desiredType, std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	return Find(desiredType, field, 2);
}

std::unique_ptr<Cell>* Vole::Find(CellTypes desiredType, std::vector<std::vector<std::unique_ptr<Cell>>>* field, int distance)
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
			if((*field)[height][width] == nullptr)
				continue;
			const auto type = (*field)[height][width]->WhatAmI();
			if (type == desiredType)
			{
				return &(*field)[height][width];
			}
		}
	}
	return nullptr;
}

void Vole::GoToDirection(int height, int width, int maxHeight, int maxWidth)
{
	if (height < -1)
		 height = -1;
	if (height > 1)
		height = 1;
	if (width < -1)
		width = -1;
	if (width > 1)
		width = 1;
	nextGenerationWidth += width;
	nextGenerationHeight += height;
	nextGenerationHeight = std::min(std::max(nextGenerationHeight, 0), maxHeight);
	nextGenerationWidth = std::min(std::max(nextGenerationWidth, 0), maxWidth);
}

void Vole::GoToRandomDirection(int maxHeight, int maxWidth)
{
	const auto height = rand() % 3 - 1;
	const auto width = rand() % 3 - 1;
	
	nextGenerationHeight += height;
	nextGenerationWidth += width;
	nextGenerationHeight = std::min(std::max(nextGenerationHeight, 0), maxHeight);
	nextGenerationWidth = std::min(std::max(nextGenerationWidth, 0), maxWidth);
}

