#pragma once
#include<vector>

class Cell {
	bool isFlagged;
	bool isMine;
	bool isOpen;

	int effectTimer;
	int minesAround;
	int mouseState;
	int x;
	int y;
public:
	Cell(int x, int y);

	bool getIsFlagged();
	bool getIsMine();
	bool getIsOpen();
	bool open(std::vector<Cell>& cells);
	bool updateEffectTimer();

	int getEffectTimer();
	int getMinesAround();
	int getMouseState();

	void countMinesAround(std::vector<Cell>& cells);
	void flag();
	void reset();
	void setEffectTimer(int effectTimer);
	void setMine();
	void setMouseState(int mouseState);
};
