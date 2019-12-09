#include "Blank.h"
#include "FemaleVole.h"

Blank::Blank(int height, int width)
{
	nextGenerationWidth = width;
	nextGenerationHeight = height;
}

void Blank::NextGeneration(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned generation, int height, int width)
{
	nextGenerationWidth = width;
	nextGenerationHeight = height;
}

CellTypes Blank::WhatAmI()
{
	return CellTypes::Blank;
}

void Blank::ResolveCollision(std::unique_ptr<Cell>* colliderPtr, std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	auto collider = colliderPtr->get();
	switch (collider->WhatAmI()) 
	{
		case CellTypes::FemaleVole:
			dynamic_cast<FemaleVole*>(collider)->GiveBirth(field, collider);
		case CellTypes::Blank:
		case CellTypes::MaleVole:
		case CellTypes::Predator:
		case CellTypes::Poison:
		default:
			(*field)[collider->nextGenerationHeight][collider->nextGenerationWidth] = std::move(*colliderPtr);
			break;
	}
}
