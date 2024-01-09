#include<random>
#include<SFML/Graphics.hpp>
#include"Cell.h"
#include"GetCell.h"
#include"Board.h"
#include"Constants.h"


Board::Board() : gameOver(0), randomEngine(randomDevice()), soundPlayed(0) {
	for (int a = 0; a < ROWS; a++) {
		for (int b = 0; b < COLUMNS; b++) {
			cells.push_back(Cell(b, a));
		}
	}
	restart();
}

bool Board::effectOver() {
	for (Cell& cell : cells) {
		if (cell.getEffectTimer() > 0) {
			return 0;
		}
	}
	return 1;
}

int Board::getGameOver() {
	return gameOver;
}

int Board::getFlags() {
	int totalFlags = 0;

	for (Cell& cell : cells) {
		totalFlags += cell.getIsFlagged();
	}
	return totalFlags;
}

void Board::draw(sf::RenderWindow& window) {
	sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

	sf::Sprite iconSprite;

	sf::Texture iconsTexture;
	iconsTexture.loadFromFile("images/tiles" + std::to_string(CELL_SIZE) + ".jpg");

	iconSprite.setTexture(iconsTexture);

	for (int a = 0; a < COLUMNS; a++) {
		for (int b = 0; b < ROWS; b++) {
			if (getCell(a, b, cells)->getIsOpen() == 1) {
				int minesAround = getCell(a, b, cells)->getMinesAround();
				iconSprite.setPosition((int)(CELL_SIZE * a),(int)(CELL_SIZE * b));
				iconSprite.setTextureRect(sf::IntRect(0, 0, CELL_SIZE, CELL_SIZE));

				if (minesAround > 0) {
					iconSprite.setPosition((int)(CELL_SIZE * a), (int)(CELL_SIZE * b));
					iconSprite.setTextureRect(sf::IntRect(CELL_SIZE * minesAround, 0, CELL_SIZE, CELL_SIZE));
				}
				window.draw(iconSprite);
			}
			else {
				iconSprite.setPosition((int)(CELL_SIZE * a), (int)(CELL_SIZE * b));
				iconSprite.setTextureRect(sf::IntRect(CELL_SIZE * 10, 0, CELL_SIZE, CELL_SIZE));

				if (getCell(a, b, cells)->getIsFlagged() == 1) {
					iconSprite.setPosition((int)(CELL_SIZE * a), (int)(CELL_SIZE * b));
					iconSprite.setTextureRect(sf::IntRect(CELL_SIZE * 11, 0, CELL_SIZE, CELL_SIZE));
				}
				window.draw(iconSprite);
			}

			getCell(a, b, cells)->setMouseState(0);

			if (gameOver != 0 && getCell(a, b, cells)->getEffectTimer() < EFFECT_DURATION) {
				int effectSize = (int)(2 * round(0.5f * CELL_SIZE * ((EFFECT_DURATION - getCell(a, b, cells)->getEffectTimer()) / (float)(EFFECT_DURATION))));

				std::uniform_int_distribution<int> effect_duration_distribution(1, EFFECT_DURATION - 1);

				cellShape.setPosition(floor(CELL_SIZE * (0.5f + a) - 0.5f * effectSize), floor(CELL_SIZE * (0.5f + b) - 0.5f * effectSize));
				cellShape.setSize(sf::Vector2f(effectSize, effectSize));

				if (gameOver == -1) {
					cellShape.setFillColor(sf::Color(255, 36, 0));
				}
				else {
					cellShape.setFillColor(sf::Color(255, 255, 255));
				}

				window.draw(cellShape);

				cellShape.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

				if (getCell(a, b, cells)->updateEffectTimer() == 1) {
					if (a - 1 >= 0 && getCell(a - 1, b, cells)->getEffectTimer() == EFFECT_DURATION) {
						getCell(a - 1, b, cells)->setEffectTimer((int)(effect_duration_distribution(randomEngine)));
					}

					if (b - 1 >= 0 && getCell(a, b - 1, cells)->getEffectTimer() == EFFECT_DURATION) {
						getCell(a, b - 1, cells)->setEffectTimer((int)(effect_duration_distribution(randomEngine)));
					}

					if (1 + a < COLUMNS && getCell(1 + a, b, cells)->getEffectTimer() == EFFECT_DURATION) {
						getCell(1 + a, b, cells)->setEffectTimer((int)(effect_duration_distribution(randomEngine)));
					}

					if (1 + b < ROWS && getCell(a, 1 + b, cells)->getEffectTimer() == EFFECT_DURATION) {
						getCell(a, 1 + b, cells)->setEffectTimer((int)(effect_duration_distribution(randomEngine)));
					}
				}
			}
		}
	}
}

void Board::flagCell(int x, int y) {
	if (gameOver == 0) {
		getCell(x, y, cells)->flag();
	}
}

void Board::openCell(int x, int y) {
	if (firstClick == 0) {
		std::uniform_int_distribution<int> xDistribution(0, COLUMNS - 1);
		std::uniform_int_distribution<int> yDistribution(0, ROWS - 1);

		firstClick = 1;

		for (int a = 0; a < MINES; a++) {
			int mineX = (int)(xDistribution(randomEngine));
			int mineY = (int)(yDistribution(randomEngine));

			if (getCell(mineX, mineY, cells)->getIsMine() == 1|| (mineX == x && mineY == y)) {
				a--;
			}
			else {
				getCell(mineX, mineY, cells)->setMine();
			}
		}

		for (Cell& cell : cells) {
			cell.countMinesAround(cells);
		}
	}

	if (gameOver == 0 && getCell(x, y, cells)->getIsFlagged() == 0) {
		if (getCell(x, y, cells)->open(cells) == 1) {
			gameOver = -1;
		}
		else {
			int totalClosedCells = 0;

			for (Cell& cell : cells) {
				totalClosedCells += 1 - cell.getIsOpen();
			}

			if (totalClosedCells == MINES) {
				gameOver = 1;
				getCell(x, y, cells)->setEffectTimer(EFFECT_DURATION - 1);
			}
		}
	}
}

void Board::restart() {
	if (gameOver != 0) {
		firstClick = 0;

		gameOver = 0;

		soundPlayed = 0;

		for (Cell& cell : cells) {
			cell.reset();
		}
	}
}

void Board::setMouseState(int mouseState, int x, int y) {
	getCell(x, y, cells)->setMouseState(mouseState);
}

bool Board::getSoundPlayed() {
	return soundPlayed;
}

void Board::setSoundPlayed(bool _soundPlayed) {
	soundPlayed = _soundPlayed;
}