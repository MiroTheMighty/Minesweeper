#include<algorithm>
#include<chrono>
#include<random>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include"Cell.h"
#include"DrawText.h"
#include"Board.h"
#include"GetCell.h"
#include"Constants.h"
#include"Minesweeper.h"
#include"Highscore.h"
#include"Menu.h"

void startGame() {

	unsigned lag = 0;
	int emojiState = 0;

	std::chrono::time_point<std::chrono::steady_clock> previousTime;
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	sf::Event event;

	sf::RenderWindow window(sf::VideoMode(CELL_SIZE * COLUMNS * SCREEN_RESIZE, SCREEN_RESIZE * (CELL_SIZE * COLUMNS + FONT_HEIGHT)), "Minesweeper", sf::Style::Close);
	window.setView(sf::View(sf::FloatRect(0, -FONT_HEIGHT, CELL_SIZE * COLUMNS, (CELL_SIZE * ROWS + FONT_HEIGHT))));

	sf::Sprite emoji;

	sf::Texture emojiTexture;
	emojiTexture.loadFromFile("images/Emoji.png");

	emoji.setPosition((int)(CELL_SIZE * (COLUMNS/2) - emojiTexture.getSize().y), -FONT_HEIGHT + 4);
	emoji.setTexture(emojiTexture);

	Board board;

	bool updateHighS = 1;

	sf::SoundBuffer clickSoundB;
	clickSoundB.loadFromFile("sounds/click.wav");
	sf::SoundBuffer loseSoundB;
	loseSoundB.loadFromFile("sounds/lose.wav");
	sf::SoundBuffer startSoundB;
	startSoundB.loadFromFile("sounds/start.wav");
	sf::SoundBuffer winSoundB;
	winSoundB.loadFromFile("sounds/win.wav");

	sf::Sound sound;

	previousTime = std::chrono::steady_clock::now();

	sound.setBuffer(startSoundB);
	sound.play();

	int minutes = 0;
	int seconds = 0;

	while (window.isOpen()) {
		unsigned deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previousTime).count();

		lag += deltaTime;

		previousTime += std::chrono::microseconds(deltaTime);

		if (board.getGameOver() == 0) {
			std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> totalSeconds = currentTime - startTime;
			minutes = (int)(totalSeconds.count()) / 60;
			seconds = (int)(totalSeconds.count()) % 60;
		}

		while (FRAME_DURATION <= lag) {
			sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
			sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
			int mouseCellX = std::clamp((int)(worldPos.x / (float)(CELL_SIZE * SCREEN_RESIZE)), 0, COLUMNS - 1);
			int mouseCellY = std::clamp((int)(worldPos.y / (float)(CELL_SIZE * SCREEN_RESIZE)), 0, ROWS - 1);

			lag -= FRAME_DURATION;

			while (window.pollEvent(event) == 1) {
				switch (event.type) {
				case sf::Event::Closed: {
					window.close();
					drawMainMenu();
					break;
				}
				case sf::Event::KeyReleased: {
					switch (event.key.code) {
					case sf::Keyboard::Return: {
						if (board.effectOver()) {
							sound.setBuffer(startSoundB);
							sound.play();
							updateHighS = 1;
							board.restart();
						}
						break;
					}
					case sf::Keyboard::Escape: {
						window.close();
						drawMainMenu();
						break;
					}
					}
					break;
				}
				case sf::Event::MouseButtonReleased: {
					switch (event.mouseButton.button) {
					case sf::Mouse::Left: {
						sound.setBuffer(clickSoundB);
						sound.play();
						board.openCell(mouseCellX, mouseCellY);
						break;
					}
					case sf::Mouse::Right: {
						sound.setBuffer(clickSoundB);
						sound.play();
						board.flagCell(mouseCellX, mouseCellY);
						break;
					}
					}
				}
				}
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == 1 || sf::Mouse::isButtonPressed(sf::Mouse::Right) == 1) {
				emojiState = 1;
				board.setMouseState(2, mouseCellX, mouseCellY);
			}
			else {
				emojiState = 0;
				board.setMouseState(1, mouseCellX, mouseCellY);
			}
			if (board.getGameOver() == -1) {
				emojiState = 3;
			}
			else if (board.getGameOver() == 1) {
				emojiState = 2;
			}

			if (lag < FRAME_DURATION) {
				sf::Color backgroundColor = sf::Color(128, 128, 128);
				window.clear(backgroundColor);

				board.draw(window);

				if (board.getGameOver() == 1 && board.getSoundPlayed() == 0) {
					sound.setBuffer(winSoundB);
					sound.play();
					board.setSoundPlayed(1);
				}
				else if (board.getGameOver() == -1 && board.getSoundPlayed() == 0) {
					sound.setBuffer(loseSoundB);
					sound.play();
					board.setSoundPlayed(1);
				}

				if (board.effectOver() == 1) {
					if (board.getGameOver() == 1) {
						drawText(1, (int)(round(0.5f * (CELL_SIZE * COLUMNS - 8 * FONT_WIDTH))), (int)(round(0.5f * (CELL_SIZE * ROWS - FONT_HEIGHT))), "VICTORY!", window);
						drawText(1, (int)(round(0.5f * (CELL_SIZE * COLUMNS - 32 * FONT_WIDTH))), (int)(round(0.5f * (CELL_SIZE * ROWS + FONT_HEIGHT * 2))), "Press the enter key to play again!", window);
						if (updateHighS) {
							updateHighscore(minutes, seconds);
							updateHighS = 0;
						}
						minutes = 0;
						seconds = 0;
						startTime = std::chrono::steady_clock::now();
					}
					else {
						drawText(1, (int)(round(0.5f * (CELL_SIZE * COLUMNS - 4 * FONT_WIDTH))), (int)(round(0.5f * (CELL_SIZE * ROWS - 2 * FONT_HEIGHT))), "GAME\n\nOVER", window);
						drawText(1, (int)(round(0.5f * (CELL_SIZE * COLUMNS - 32 * FONT_WIDTH))), (int)(round(0.5f * (CELL_SIZE * ROWS + FONT_HEIGHT * 2))), "Press the enter key to play again!", window);
						minutes = 0;
						seconds = 0;
						startTime = std::chrono::steady_clock::now();
					}
				}

				drawText(0, 0, -FONT_HEIGHT, "Mines:" + std::to_string(MINES - board.getFlags()), window);
				drawText(0, (CELL_SIZE * ROWS) - (FONT_WIDTH * 10), -FONT_HEIGHT, "Time:" + std::to_string(minutes) + ":" + std::to_string(seconds), window);

				emoji.setTextureRect(sf::IntRect(emojiState * emojiTexture.getSize().y, 0, emojiTexture.getSize().y, emojiTexture.getSize().y));

				window.draw(emoji);
				window.display();
			}
		}
	}
}