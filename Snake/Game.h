#pragma once
#include "SFML/Graphics.hpp"

class Game
{
public:
	Game()=default;
	~Game()=default;

	void init();

private:
	void mainLoop();
	void handleEvents();
	void render();

	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<sf::CircleShape> shape;
};

