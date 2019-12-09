#include "Poison.h"
#include <iostream>
#include "Vole.h"
#include "FemaleVole.h"

Poison::Poison(int height, int width)
{
	nextGenerationHeight = height;
	nextGenerationWidth = width;
}

void Poison::NextGeneration(std::vector<std::vector<std::unique_ptr<Cell>>>* field, unsigned generation, int height, int width)
{
	nextGenerationHeight = height;
	nextGenerationWidth = width;

}

CellTypes Poison::WhatAmI()
{
	return CellTypes::Poison;
}

void Poison::ResolveCollision(std::unique_ptr<Cell>* colliderPtr, std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	auto collider = colliderPtr->get();
	switch (collider->WhatAmI())
	{
	case CellTypes::FemaleVole:
		dynamic_cast<FemaleVole*>(collider)->GiveBirth(field, collider);
	case CellTypes::MaleVole:
		dynamic_cast<Vole*>(collider)->WillDie = true;
	case CellTypes::Predator:
		(*field)[collider->nextGenerationHeight][collider->nextGenerationWidth] = std::move(*colliderPtr);
		break;
	case CellTypes::Blank:
	case CellTypes::Poison:
	default:
		break;
	}
}
