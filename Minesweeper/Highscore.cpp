#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
#include<SFML/Graphics.hpp>
#include"Highscore.h"
#include"Constants.h"

void updateHighscore(int _minutes, int _seconds) {
    std::vector<std::pair<std::string, double>> highscores;
    std::ifstream inputFile("highscores.txt", std::ios::in);

    if (!inputFile.is_open()) {
        std::ofstream createFile("highscores.txt", std::ios::app);
        if (createFile.is_open()) {
            createFile.close();
            inputFile.open("highscores.txt", std::ios::in);
        }
        else {
            std::cerr << "Error creating the file!\n";
        }
    }

    std::string name;
    double minutes, seconds;
    bool isHighscore = false;
    int currentTimeInSeconds = _minutes * 60 + _seconds;

    if (inputFile.peek() == std::ifstream::traits_type::eof()) {
        isHighscore = true;
    }
    else {
        while (inputFile >> name >> minutes >> seconds) {
            highscores.push_back({ name, minutes * 60 + seconds });
        }
        isHighscore = std::any_of(highscores.begin(), highscores.end(), [currentTimeInSeconds](const auto& score) {
            return currentTimeInSeconds < score.second;
            });
    }

    inputFile.close();

    if (isHighscore) {
        std::string playerName = drawHighscoreWindow(1);
        std::ofstream outputFile("highscores.txt", std::ios::app);

        if (outputFile.is_open()) {
            outputFile << playerName << " " << _minutes << ":" << _seconds << std::endl;
            outputFile.close();
        }
        else {
            std::cerr << "Error opening output file!\n";
        }
    }
    else {
        std::string playerName = drawHighscoreWindow(0);
        std::ofstream outputFile("highscores.txt", std::ios::app);

        if (outputFile.is_open()) {
            outputFile << playerName << " " << _minutes << ":" << _seconds << std::endl;
            outputFile.close();
        }
        else {
            std::cerr << "Error opening output file!\n";
        }
    }
}

bool isMouseOverButton(const sf::RectangleShape& button, const sf::RenderWindow& window) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    return buttonBounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
}

std::string drawHighscoreWindow(bool highScore) {
    sf::RenderWindow window(sf::VideoMode(320, 100), highScore ? "Highscore!" : "Not highscore :(");

    sf::Font font;
    font.loadFromFile("images/Louis George Cafe.ttf");

    sf::Text prompt("Enter your name:", font, 20);
    prompt.setPosition(10.f, 10.f);
    prompt.setFillColor(sf::Color::Black);

    sf::RectangleShape inputBox(sf::Vector2f(200.f, 30.f));
    inputBox.setPosition(10.f, 50.f);
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setFillColor(sf::Color(240, 240, 240));

    sf::Text playerName("", font, 20);
    playerName.setPosition(15.f, 55.f);
    playerName.setFillColor(sf::Color::Black);


    sf::RectangleShape button(sf::Vector2f(80.f, 30.f));
    button.setPosition(220.f, 50.f);
    button.setOutlineThickness(2.f);
    button.setOutlineColor(sf::Color::Black);
    button.setFillColor(sf::Color(200, 200, 200));
    sf::Text buttonText("Submit", font, 15);
    buttonText.setPosition(235.f, 60.f);
    buttonText.setFillColor(sf::Color::Black);

    std::string inputText;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128 && event.text.unicode != 8 && event.text.unicode != 13) {
                    inputText += static_cast<char>(event.text.unicode);
                    playerName.setString(inputText);
                }
                else if (event.text.unicode == 8 && !inputText.empty()) {
                    inputText.pop_back();
                    playerName.setString(inputText);
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased && isMouseOverButton(button, window)) {
                if (!inputText.empty()) {
                    window.close();
                }
            }
        }

        if (isMouseOverButton(button, window) && !inputText.empty()) {
            button.setFillColor(sf::Color(220, 220, 220));
        }
        else {
            button.setFillColor(sf::Color(200, 200, 200));
        }

        window.clear(sf::Color::White);

        window.draw(prompt);
        window.draw(inputBox);
        window.draw(playerName);

        window.draw(button);
        window.draw(buttonText);

        window.display();
    }

    return playerName.getString().toAnsiString();
}