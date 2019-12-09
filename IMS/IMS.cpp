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

int fieldHeight = 150;
int fieldWidth = 150;
int poisonSpawnGeneration = -1;
int poisonCount = -1;
int maleCount = -1;
int femaleCount = -1;
int predatorCount = -1;
int predatorSuccessRate = -1;
int numberOfSimulatedDays = -1;

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
			auto pregnant = false;

			if (male)
			{

				*cell = std::make_unique<MaleVole>(1, i, j);
			}
			else
			{
				*cell = std::make_unique<FemaleVole>(1, i, j);
				auto female = dynamic_cast<FemaleVole*>((*field)[i][j].get());
				pregnant = (rand() % 100) < 50;
				if (pregnant)
				{
					female->birthGeneration = rand() % female->PregnantTime;
					female->isPregnant = true;
				}
			}

			if ((male || !pregnant) && (rand() % 100) < 20)
			{

				auto vole = dynamic_cast<Vole*>((*field)[i][j].get());
				vole->isAdult = false;
				if (male)
				{
					vole->adultGeneration = rand() % MaleVole::ChildTime;
				}
				else
				{
					vole->adultGeneration = rand() % FemaleVole::ChildTime;
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

void GetSettings(bool useDefaults)
{
	if (useDefaults)
	{
		fieldHeight = 150;
		fieldWidth = 150;
		poisonSpawnGeneration = -1;
		poisonCount = 20;
		maleCount = 10;
		femaleCount = 13;
		predatorCount = 1;
		predatorSuccessRate = 73;
		numberOfSimulatedDays = 93;
		return;
	}
	std::cout << "Enter number of male voles:" << std::endl;
	std::cin >> maleCount;
	if (maleCount < 0)
	{
		std::cerr << "Incorrect value entered.";
		exit(EXIT_FAILURE);
	}

	std::cout << "Enter number of female voles:" << std::endl;
	std::cin >> femaleCount;
	if (femaleCount < 0)
	{
		std::cerr << "Incorrect value entered.";
		exit(EXIT_FAILURE);
	}

	std::cout << "Enter how many poison baits will be set (0 for no baits):" << std::endl;
	std::cin >> poisonCount;
	if (poisonCount < 0)
	{
		std::cerr << "Incorrect value entered.";
		exit(EXIT_FAILURE);
	}

	if (poisonCount > 0)
	{
		std::cout << "Enter when (days from the start of the simulation) the poison baits will be set (1 for immediate):" << std::endl;
		std::cin >> poisonSpawnGeneration;
		if (poisonSpawnGeneration < 1)
		{
			std::cerr << "Incorrect value entered.";
			exit(EXIT_FAILURE);
		}
	}

	std::cout << "Enter number of predators:" << std::endl;
	std::cin >> predatorCount;
	if (predatorCount < 0)
	{
		std::cerr << "Incorrect value entered.";
		exit(EXIT_FAILURE);
	}

	if (predatorCount > 0)
	{
		int habitat = -1;
		std::cout << "Enter simulated habitat of common vole:" << std::endl;
		std::cout << "1) Plowed fields, light stubble, newly planted crops" << std::endl;
		std::cout << "2) Disturbed grassland and fields" << std::endl;
		std::cout << "3) Heavy, tall stubble field" << std::endl;
		std::cout << "4) Idle, undisturbed pastures and meadow" << std::endl;
		std::cout << "5) Crops (corn, wheat, beans,milo, oats)" << std::endl;
		std::cout << "6) Old fields, overgrown pastures and meadow" << std::endl;
		std::cout << "7) Woodlots" << std::endl;
		std::cin >> habitat;

		switch (habitat)
		{
		case 1:
			predatorSuccessRate = 74;
			break;

		case 2:
			predatorSuccessRate = 83;
			break;

		case 3:
			predatorSuccessRate = 65;
			break;
		case 4:
			predatorSuccessRate = 53;
			break;
		case 5:
			predatorSuccessRate = 42;
			break;
		case 6:
			predatorSuccessRate = 33;
			break;
		case 7:
			predatorSuccessRate = 33;
			break;
		default:
			std::cerr << "Incorrect value entered.";
			exit(EXIT_FAILURE);
		}
	}

	std::cout << "Enter number of simulated days:" << std::endl;
	std::cin >> numberOfSimulatedDays;
	if (numberOfSimulatedDays < 1)
	{
		std::cerr << "Incorrect value entered.";
		exit(EXIT_FAILURE);
	}

}

int main()
{
	GetSettings(false);
	srand(time(NULL));

	std::vector<std::vector<std::unique_ptr<Cell>>> field(fieldHeight);
	std::vector<std::vector<std::unique_ptr<Cell>>> tmpField(fieldHeight);
	InitFields(fieldWidth, fieldHeight, &field, &tmpField);

	SpawnVoles(maleCount, &field, true);
	SpawnVoles(femaleCount, &field, false);
	SpawnPredators(predatorCount, &field);

	for (int generation = 1; generation < 24 * numberOfSimulatedDays; ++generation)
	{
		if (generation == poisonSpawnGeneration * 24)
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
