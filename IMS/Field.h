#pragma once
#include "Cell.h"
#include <memory>
#include <vector>

class Field
{
private:
	int 
public:
	Field(int fieldHeight, int fieldWidth);
	std::vector<std::vector<std::unique_ptr<Cell>>> field(fieldHeight);
	std::vector<std::vector<std::unique_ptr<Cell>>> tmpField(fieldHeight);
};

