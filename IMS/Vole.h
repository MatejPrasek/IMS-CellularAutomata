#pragma once
#include "Cell.h"

class Vole :
	public Cell
{
public:
	bool isAdult;
	bool WillDie;

	Vole(unsigned int adultGen, int height, int width);
	void ResolveCollision(std::unique_ptr<Cell>* colliderPtr, std::vector<std::vector<std::unique_ptr<Cell>>>* field) override;
	unsigned int adultGeneration;
protected:
	std::unique_ptr<Cell>* FindClose(CellTypes desiredType, std::vector<std::vector<std::unique_ptr<Cell>>>* field);
	std::unique_ptr<Cell>* FindFar(CellTypes desiredType, std::vector<std::vector<std::unique_ptr<Cell>>>* field);
	std::unique_ptr<Cell>* Find(CellTypes desiredType, std::vector<std::vector<std::unique_ptr<Cell>>>* field, int distance);
	void GoToDirection(int height, int width, int maxHeight, int maxWidth);
	void GoToRandomDirection(int maxHeight, int maxWidth);
};
