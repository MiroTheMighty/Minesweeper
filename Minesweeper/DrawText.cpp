#include<SFML/Graphics.hpp>
#include"DrawText.h"

void drawText(bool black, int x, int y, const std::string& text, sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("images/Louis George Cafe.ttf")) {
        return;
    }

    sf::Text textObject;
    textObject.setFont(font);
    textObject.setCharacterSize(20);
    textObject.setFillColor(black ? sf::Color::Black : sf::Color::White);
    textObject.setPosition(x, y);

    for (const char& c : text) {
        if (c == '\n') {
            textObject.setPosition(x, textObject.getPosition().y + textObject.getLocalBounds().height);
            continue;
        }

        textObject.setString(c);
        window.draw(textObject);
        textObject.move(textObject.getLocalBounds().width, 0);
    }
}