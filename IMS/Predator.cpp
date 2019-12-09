#include "Predator.h"
#include "Vole.h"
#include <algorithm>

Predator::Predator(int height, int width, int successChance)
{
	fedUntilGeneration = 0;
	nextGenerationHeight = height;
	nextGenerationWidth = width;
	sucessChance = successChance;
}

void Predator::NextGeneration(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned generation, int height, int width)
{
	nextGenerationHeight = height;
	nextGenerationWidth = width;
	GetDirection(field, generation);
}

CellTypes Predator::WhatAmI()
{
	return CellTypes::Predator;
}

void Predator::ResolveCollision(std::unique_ptr<Cell>* colliderPtr, std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	auto collider = colliderPtr->get();
	std::unique_ptr<Cell>* target;
	switch (collider->WhatAmI())
	{
		// Copy it on neighbor cell
	case CellTypes::Predator:
		target = GetCellToPlace(field, collider);
		if (target != nullptr)
			target->get()->ResolveCollision(colliderPtr, field);
		break;
		// Do not copy 
	case CellTypes::Blank:
	case CellTypes::MaleVole:
	case CellTypes::FemaleVole:
	case CellTypes::Poison:
	default:
		break;
	}
}

std::unique_ptr<Cell>* Predator::GetCellToPlace(std::vector<std::vector<std::unique_ptr<Cell>>>* field, Cell* collider)
{
	const int maxHeight = (*field).size() - 1;
	const int maxWidth = (*field)[0].size() - 1;
	std::unique_ptr<Cell>* toReturn = nullptr;
	for (auto i = -1; i <= 1; ++i)
	{
		const auto height = i < 0 ? std::max(collider->nextGenerationHeight + i, 0) : std::min(collider->nextGenerationHeight + i, maxHeight);
		for (auto j = -1; j <= 1; ++j)
		{
			const auto width = j < 0 ? std::max(collider->nextGenerationWidth + j, 0) : std::min(collider->nextGenerationWidth + j, maxWidth);
			const auto type = (*field)[height][width]->WhatAmI();
			if (type == CellTypes::Blank)
			{
				return &(*field)[height][width];
			}
			else if (type == CellTypes::MaleVole || type == CellTypes::FemaleVole)
			{
				toReturn = &(*field)[height][width];
			}
			else if (toReturn == nullptr && type == CellTypes::Poison)
			{
				toReturn = &(*field)[height][width];
			}
		}
	}
	return toReturn;
}

void Predator::GetDirection(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned generation)
{
	const int maxHeight = (*field).size() - 1;
	const int maxWidth = (*field)[0].size() - 1;
	auto closestHeight = maxHeight;
	auto closestWidth = maxWidth;

	if (fedUntilGeneration <= generation)
	{
		for (auto i = -5; i <= 5; ++i)
		{
			const auto height = i < 0 ? std::max(nextGenerationHeight + i, 0) : std::min(nextGenerationHeight + i, maxHeight);
			for (auto j = -5; j <= 5; ++j)
			{
				const auto width = j < 0 ? std::max(nextGenerationWidth + j, 0) : std::min(nextGenerationWidth + j, maxWidth);
				const auto type = (*field)[height][width]->WhatAmI();
				if (type == CellTypes::MaleVole || type == CellTypes::FemaleVole)
				{
					if (std::abs(closestHeight) > std::abs(i) && std::abs(closestWidth) > std::abs(j))
					{
						closestHeight = i;
						closestWidth = j;
					}
				}
			}
		}
	}
	// vole found
	if (closestHeight < maxHeight || closestWidth < maxWidth)
	{
		if (std::abs(closestHeight) <= 2 && std::abs(closestWidth) <= 2)
		{
			if ((rand() % 100) < sucessChance)
			{
				dynamic_cast<Vole*>((*field)[nextGenerationHeight + closestHeight][nextGenerationWidth + closestWidth].get())->WillDie = true;
				fedUntilGeneration = generation + fedDuration;
			}
			else
			{
				fedUntilGeneration = generation + failedHuntRestorationDuration;
			}
		}
		else
		{
			if (closestHeight < -2)
				closestHeight = -2;
			if (closestHeight > 2)
				closestHeight = 2;
			if (closestWidth < -2)
				closestWidth = -2;
			if (closestWidth > 2)
				closestWidth = 2;
		}
	}
	// no vole found
	else
	{
		closestHeight = rand() % 5 - 2;
		closestWidth = rand() % 5 - 2;
	}
	nextGenerationHeight += closestHeight;
	nextGenerationWidth += closestWidth;
	nextGenerationHeight = std::min(std::max(nextGenerationHeight, 0), maxHeight);
	nextGenerationWidth = std::min(std::max(nextGenerationWidth, 0), maxWidth);
}
