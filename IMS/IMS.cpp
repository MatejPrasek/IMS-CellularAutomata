#include "Cell.h"
#include "MaleVole.h"
#include "FemaleVole.h"
#include "Blank.h"
#include <memory>
#include <vector>
#include "Predator.h"
#include <iostream>
#include "Poison.h"

void CreateNewGeneration(int fieldWidth, int fieldHeight, std::vector<std::vector<std::unique_ptr<Cell>>>* field, std::vector<std::vector<std::unique_ptr<Cell>>>* tmpField)
{
	for (int i = 0; i < fieldHeight; ++i)
	{
		for (int j = 0; j < fieldWidth; ++j)
		{
			if ((*tmpField)[i][j] == nullptr)
				(*field)[i][j] = std::make_unique<Blank>(i,j);
			else
				(*field)[i][j] = std::move((*tmpField)[i][j]);
			(*tmpField)[i][j] = std::make_unique<Blank>(i,j);
		}
	}
}

void InitFields(int fieldWidth, int fieldHeight, std::vector<std::vector<std::unique_ptr<Cell>>>* field, std::vector<std::vector<std::unique_ptr<Cell>>>* tmpField)
{
	for (int i = 0; i < fieldHeight; ++i)
	{
		(*field)[i] = std::vector<std::unique_ptr<Cell>>(fieldWidth);
		(*tmpField)[i] = std::vector<std::unique_ptr<Cell>>(fieldWidth);
		for (int j = 0; j < fieldWidth; ++j)
		{
			(*field)[i][j] = std::make_unique<Blank>(i,j);
			(*tmpField)[i][j] = std::make_unique<Blank>(i,j);
		}
	}
}

void Move(int fieldHeight, int fieldWidth, std::vector<std::vector<std::unique_ptr<Cell>>>* field, std::vector<std::vector<std::unique_ptr<Cell>>>* tmpField)
{
	for (int i = 0; i < fieldHeight; ++i)
	{
		for (int j = 0; j < fieldWidth; ++j)
		{
			if((*field)[i][j] == nullptr)
			{
				continue;
			}
			(*tmpField)[(*field)[i][j]->nextGenerationHeight][(*field)[i][j]->nextGenerationWidth]->ResolveCollision(&(*field)[i][j], &(*tmpField));
		}
	}
}

void Calculate(int fieldHeight, int fieldWidth, std::vector<std::vector<std::unique_ptr<Cell>>>* field, int generation)
{
	for (int i = 0; i < fieldHeight; ++i)
	{
		for (int j = 0; j < fieldWidth; ++j)
		{
			(*field)[i][j]->NextGeneration(&(*field), generation, i, j);
		}
	}
}

void SpawnPoison(int poisonCount, std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	int maxHeight = field->size() - 1;
	int maxWidth = field[0].size() - 1;
	while(poisonCount > 0)
	{
		int i = rand() % (maxHeight + 1);
		int j = rand() % (maxWidth + 1);
		std::unique_ptr<Cell>* cell = &(*field)[i][j];
		if (cell->get()->WhatAmI() == CellTypes::Blank)
		{
			*cell = std::make_unique<Poison>(i, j);
			poisonCount--;
		}
	}
}

int main()
{
	int fieldHeight = 10;
	int fieldWidth = 10;
	int poisonSpawnGeneration = 30 * 24;
	int poisonCount = 20;

	std::vector<std::vector<std::unique_ptr<Cell>>> field(fieldHeight);
	std::vector<std::vector<std::unique_ptr<Cell>>> tmpField(fieldHeight);
	InitFields(fieldWidth, fieldHeight, &field, &tmpField);


	//field[8][9] = std::make_unique<Predator>(8,9);
	field[5][5] = std::make_unique<MaleVole>(1,5,5);
	field[3][4] = std::make_unique<FemaleVole>(1,3,4);
	field[9][8] = std::make_unique<MaleVole>(1,9,8);
	field[8][8] = std::make_unique<MaleVole>(1,8,8);
	field[7][8] = std::make_unique<MaleVole>(1,7,8);
	field[6][8] = std::make_unique<MaleVole>(1,6,8);
	field[5][8] = std::make_unique<FemaleVole>(1,5,8);
	field[4][8] = std::make_unique<MaleVole>(1,4,8);
	field[3][8] = std::make_unique<MaleVole>(1,3,8);
	
	for (int generation = 1; generation < 2400; ++generation)
	{
		if (generation == poisonSpawnGeneration)
			SpawnPoison(poisonCount, &field);
		Calculate(fieldHeight, fieldWidth, &field, generation);
		Move(fieldHeight, fieldWidth, &field, &tmpField);
		CreateNewGeneration(fieldWidth, fieldHeight, &field, &tmpField);


		//debug listing
		if(generation%24 != 0)
			continue;
		std::cout << std::endl << "Generation " << generation/24 << std::endl;
		for (int i = 0; i < fieldHeight; ++i)
		{
			for (int j = 0; j < fieldWidth; ++j)
			{
				if (field[i][j]->WhatAmI() == CellTypes::FemaleVole || field[i][j]->WhatAmI() == CellTypes::MaleVole)
				{
					std::cout << i << j << std::endl;
				}
			}
		}
	}
	return 0;
}
