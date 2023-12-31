#include"SFML/Graphics.hpp"
#include"Menu.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(400, 270),"Menu");
	drawMainMenu(window);
	
	return 0;
}