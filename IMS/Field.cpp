#include "Field.h"
#include "Blank.h"
#include <memory>
#include <vector>

Field::Field(int fieldHeight, int fieldWidth)
{
	for (int i = 0; i < fieldHeight; ++i)
	{
		field[i] = std::vector<std::unique_ptr<Cell>>(fieldWidth);
		tmpField[i] = std::vector<std::unique_ptr<Cell>>(fieldWidth);
		for (int j = 0; j < fieldWidth; ++j)
		{
			field[i][j] = std::make_unique<Blank>();
			tmpField[i][j] = std::make_unique<Blank>();
		}
	}
}
