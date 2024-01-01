#include"Cell.h"
#include"GetCell.h"
#include"Constants.h"
#include<vector>

Cell* getCell(int x, int y, std::vector<Cell>& cells) {
	return &cells[x + COLUMNS * y];
}