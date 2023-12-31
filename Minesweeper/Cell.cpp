#include "Cell.h"
#include "GetCell.h"
#include "Constants.h"

Cell::Cell(int _x, int _y) : minesAround(0), x(_x), y(_y) {
	reset();
}

bool Cell::getIsFlagged() {
	return isFlagged;
}

bool Cell::getIsMine() {
	return isMine;
}

bool Cell::getIsOpen() {
	return isOpen;
}

bool Cell::open(std::vector<Cell>& cells) {
	if (0 == isOpen) {
		isOpen = 1;
		if (isMine == 0 && minesAround == 0) {
			for (int a = -1; a < 2; a++) {
				for (int b = -1; b < 2; b++) {
					if ((0 == a && 0 == b) || (0 > a + x || 0 > b + y || COLUMNS == a + x || ROWS == b + y)) {
						continue;
					}
					getCell(a + x, b + y, cells)->open(cells);
				}
			}
		}
		effectTimer -= isMine;

		return isMine;
	}
	return 0;
}

bool Cell::updateEffectTimer() {
	if (effectTimer > 0) {
		effectTimer--;
		if (effectTimer == 0) {
			return 1;
		}
	}

	return 0;
}

int Cell::getEffectTimer() {
	return effectTimer;
}

int Cell::getMinesAround() {
	return minesAround;
}

int Cell::getMouseState() {
	return mouseState;
}

void Cell::countMinesAround(std::vector<Cell>& cells) {
	minesAround = 0;

	if (0 == isMine) {
		for (int a = -1; a < 2; a++) {
			for (int b = -1; b < 2; b++) {
				if ((a == 0 && b == 0) || (a + x < 0|| b + y < 0|| a + x == COLUMNS || b + y == ROWS)) {
					continue;
				}
				if (getCell(a + x, b + y, cells)->getIsMine() == 1) {
					minesAround++; 
				}
			}
		}
	}
}

void Cell::flag() {
	if (isOpen == 0) {
		isFlagged = !isFlagged;
	}
}

void Cell::reset() {
	isFlagged = 0;
	isMine = 0;
	isOpen = 0;

	effectTimer = EFFECT_DURATION;
	mouseState = 0;
}

void Cell::setEffectTimer(int _effect_timer) {
	effectTimer = _effect_timer;
}

void Cell::setMine() {
	isMine = 1;
}

void Cell::setMouseState(int _mouse_state) {
	mouseState = _mouse_state;
}