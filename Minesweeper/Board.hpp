#pragma once

class Board {
	bool firstClick;
	int gameOver;
	bool soundPlayed;
	std::default_random_engine randomEngine;
	std::random_device randomDevice;
	std::vector<Cell> cells;
public:
	Board();
	bool effectOver();
	int getGameOver();
	int getFlags();
	void draw(sf::RenderWindow& window);
	void flagCell(int x, int y);
	void openCell(int x, int y);
	void restart();
	void setMouseState(int mouseState, int x, int y);
	bool getSoundPlayed();
	void setSoundPlayed(bool soundPlayed);
};
