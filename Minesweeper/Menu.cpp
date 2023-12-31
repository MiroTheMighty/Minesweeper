#include<SFML/Graphics.hpp>
#include<fstream>
#include<iostream>
#include<vector>
#include<algorithm>
#include<iomanip>
#include<sstream>
#include"Menu.h"
#include"Minesweeper.h"

std::vector<std::pair<std::string, int>> readHighscores() {
    std::ifstream file("highscores.txt");
    std::vector<std::pair<std::string, int>> highscores;

    if (!file.is_open()) {
        std::cerr << "Error opening highscore file!" << std::endl;
        return highscores;
    }

    std::string name;
    char colon;
    int minutes, seconds;

    while (file >> name >> minutes >> colon >> seconds) {
        if (colon != ':') {
            std::cerr << "Error reading highscore file: Invalid format!" << std::endl;
            highscores.clear();
            break;
        }

        int totalSeconds = minutes * 60 + seconds;

        highscores.push_back({ name, totalSeconds });
    }

    if (file.fail() && !file.eof()) {
        std::cerr << "Error reading highscore file!" << std::endl;
    }

    file.close();

    std::sort(highscores.begin(), highscores.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
        });

    return highscores;
}

bool drawHighscoreMenu(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("images/Louis George Cafe.ttf");

    sf::Text title("Highscores", font, 30);
    title.setPosition(125, 10);
    title.setFillColor(sf::Color::Black);

    std::vector<std::pair<std::string, int>> highscores = readHighscores();

    sf::Text highscoreText("", font, 20);
    highscoreText.setPosition(140, 60);
    highscoreText.setFillColor(sf::Color::Black);

    for (size_t i = 0; i < std::min(highscores.size(), static_cast<size_t>(8)); ++i) {
        int minutes = highscores[i].second / 60;
        int seconds = highscores[i].second % 60;
        std::ostringstream formattedTime;
        formattedTime << highscores[i].first << " - " << std::setw(3) << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
        highscoreText.setString(highscoreText.getString() + formattedTime.str() + "\n");
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    return true;
                }
            }
        }

        window.clear(sf::Color::White);

        window.draw(title);
        window.draw(highscoreText);

        window.display();
    }
    return false;
}

void drawMainMenu(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("images/Louis George Cafe.ttf");

    sf::Text title("Minesweeper", font, 30);
    title.setPosition(125, 50);
    title.setFillColor(sf::Color::Black);

    sf::Text playOption("Play", font, 20);
    playOption.setPosition(155, 120);
    playOption.setFillColor(sf::Color::Black);

    sf::Text highscoreOption("Highscores", font, 20);
    highscoreOption.setPosition(155, 160);
    highscoreOption.setFillColor(sf::Color::Black);

    sf::Text exitOption("Exit", font, 20);
    exitOption.setPosition(155, 200);
    exitOption.setFillColor(sf::Color::Black);

    int selectedOption = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    selectedOption = (selectedOption + 1) % 3;
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    selectedOption = (selectedOption + 2) % 3;
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (selectedOption == 0) {
                        startGame();
                    }
                    else if (selectedOption == 1) {
                        if (drawHighscoreMenu(window)) {
                            break;
                        }
                    }
                    else if (selectedOption == 2) {
                        window.close();
                    }
                }
            }
        }

        playOption.setFillColor(selectedOption == 0 ? sf::Color::Red : sf::Color::Black);
        highscoreOption.setFillColor(selectedOption == 1 ? sf::Color::Red : sf::Color::Black);
        exitOption.setFillColor(selectedOption == 2 ? sf::Color::Red : sf::Color::Black);

        window.clear(sf::Color::White);

        window.draw(title);
        window.draw(playOption);
        window.draw(highscoreOption);
        window.draw(exitOption);

        window.display();
    }
}