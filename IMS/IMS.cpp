#include "Cell.h"
#include "MaleVole.h"
#include "FemaleVole.h"
#include "Blank.h"
#include <memory>
#include <vector>
#include "Predator.h"
#include <iostream>
#include "Poison.h"
#include <time.h>

int fieldHeight = 80;
int fieldWidth = 80;
int poisonSpawnGeneration = -1;
int poisonCount = 20;
int maleCount = 10;
int femaleCount = 13;
int predatorCount = 1;
int predatorSuccessRate = 42;

void CreateNewGeneration(int fieldWidth, int fieldHeight, std::vector<std::vector<std::unique_ptr<Cell>>>* field, std::vector<std::vector<std::unique_ptr<Cell>>>* tmpField)
{
	for (int i = 0; i < fieldHeight; ++i)
	{
		for (int j = 0; j < fieldWidth; ++j)
		{
			if ((*tmpField)[i][j] == nullptr)
				(*field)[i][j] = std::make_unique<Blank>(i, j);
			else
				(*field)[i][j] = std::move((*tmpField)[i][j]);
			(*tmpField)[i][j] = std::make_unique<Blank>(i, j);
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
			(*field)[i][j] = std::make_unique<Blank>(i, j);
			(*tmpField)[i][j] = std::make_unique<Blank>(i, j);
		}
	}
}

void Move(int fieldHeight, int fieldWidth, std::vector<std::vector<std::unique_ptr<Cell>>>* field, std::vector<std::vector<std::unique_ptr<Cell>>>* tmpField)
{
	for (int i = 0; i < fieldHeight; ++i)
	{
		for (int j = 0; j < fieldWidth; ++j)
		{
			if ((*field)[i][j] == nullptr)
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
	while (poisonCount > 0)
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
void SpawnVoles(int count, std::vector<std::vector<std::unique_ptr<Cell>>>* field, bool male)
{
	int maxHeight = field->size() - 1;
	int maxWidth = field[0].size() - 1;
	while (count > 0)
	{
		int i = rand() % (maxHeight + 1);
		int j = rand() % (maxWidth + 1);
		std::unique_ptr<Cell>* cell = &(*field)[i][j];
		if (cell->get()->WhatAmI() == CellTypes::Blank)
		{
			if (male)
			{

				*cell = std::make_unique<MaleVole>(1, i, j);
			}
			else
			{
				*cell = std::make_unique<FemaleVole>(1, i, j);
				auto female = dynamic_cast<FemaleVole*>((*field)[i][j].get());
				if ((rand() % 100) < 50)
				{
					female->birthGeneration = rand() % female->PregnantTime;
					female->isPregnant = true;
				}
			}
			count--;
		}
	}
}
void SpawnPredators(int count, std::vector<std::vector<std::unique_ptr<Cell>>>* field)
{
	int maxHeight = field->size() - 1;
	int maxWidth = field[0].size() - 1;
	while (count > 0)
	{
		int i = rand() % (maxHeight + 1);
		int j = rand() % (maxWidth + 1);
		std::unique_ptr<Cell>* cell = &(*field)[i][j];
		if (cell->get()->WhatAmI() == CellTypes::Blank)
		{
			*cell = std::make_unique<Predator>(i, j, predatorSuccessRate);
			count--;
		}
	}
}

int main()
{
	srand(time(NULL));

	std::vector<std::vector<std::unique_ptr<Cell>>> field(fieldHeight);
	std::vector<std::vector<std::unique_ptr<Cell>>> tmpField(fieldHeight);
	InitFields(fieldWidth, fieldHeight, &field, &tmpField);

	SpawnVoles(maleCount, &field, true);
	SpawnVoles(femaleCount, &field, false);
	SpawnPredators(predatorCount, &field);

	for (int generation = 1; generation < 2400; ++generation)
	{
		if (generation == poisonSpawnGeneration)
			SpawnPoison(poisonCount, &field);
		Calculate(fieldHeight, fieldWidth, &field, generation);
		Move(fieldHeight, fieldWidth, &field, &tmpField);
		CreateNewGeneration(fieldWidth, fieldHeight, &field, &tmpField);


		int adults = 0;
		int nonAdults = 0;
		//debug listing
		if (generation % 24 != 0)
			continue;
		std::cout << std::endl << "Generation " << generation / 24 << std::endl;
		for (int i = 0; i < fieldHeight; ++i)
		{
			for (int j = 0; j < fieldWidth; ++j)
			{
				if (field[i][j]->WhatAmI() == CellTypes::FemaleVole || field[i][j]->WhatAmI() == CellTypes::MaleVole)
				{
					auto vole = dynamic_cast<Vole*>(field[i][j].get());
					//std::cout << i << "|" << j << "		" << vole->isAdult << std::endl;
					if (vole->isAdult)
					{
						adults++;
					}
					else
					{
						nonAdults++;
					}
				}
			}
		}

		std::cout << "adults: " << adults << std::endl;
		std::cout << "non adults: " << nonAdults << std::endl;
	}
	return 0;
}
