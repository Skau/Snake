#include "Game.h"

void Game::init()
{
	window = std::make_unique<sf::RenderWindow>(sf::VideoMode(200, 200), "Snake");
	shape = std::make_unique<sf::CircleShape>(sf::CircleShape(100.f));
	shape->setFillColor(sf::Color::Green);

	mainLoop();
}

void Game::mainLoop()
{
	while (window->isOpen())
	{
		handleEvents();
		render();
	}
}

void Game::handleEvents()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
	}
}

void Game::render()
{
	window->clear();
	window->draw(*shape);
	window->display();
}
