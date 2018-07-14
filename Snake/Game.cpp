#include "Game.h"
#include <memory>
#include <iostream>
#include <time.h>
#include "Snake.h"
#include "Dot.h"

Game::Game()
{
	srand((int)time(NULL));

	window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 800), "Snake");
	window->setFramerateLimit(120);
}

void Game::init()
{
	snake = std::make_shared<Snake>();

	dotImage = std::make_unique<sf::Image>(sf::Image());

	if (!dotImage->loadFromFile("images/redImage.png"))
	{
		std::cerr << "Could not load red image!\n";
	}

	dot = std::make_shared<Dot>(*dotImage, getRandomPos());

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	mainLoop();
}

void Game::mainLoop()
{
	while (window->isOpen())
	{
		handleEvents();
		mainTick();
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

		snake->handleEvents(event);
	}
}

void Game::mainTick()
{
	timeSinceLastUpdate += frameClock.restart();
	while (timeSinceLastUpdate > timePerFrame)
	{
		timeSinceLastUpdate -= timePerFrame;

		if (!dot.get())
		{
			dot = std::make_shared<Dot>(*dotImage, getRandomPos());
		}

		if (snake->getHeadNode().getColBox().getGlobalBounds().intersects(dot->getColBox().getGlobalBounds()))
		{
			dot.reset();
			snake->spawnNewNode();
		}
		
		//for (auto node : snake->getNodes())
		//{
		//	if (node->getColBox().getGlobalBounds().intersects(dot->getColBox().getGlobalBounds()))
		//	{
		//		dot.reset();
		//	}
		//}

		snake->tick(timePerFrame.asSeconds());
	}

}

void Game::render()
{
	window->clear();

	snake->render(*window);

	if(dot.get())
	dot->render(*window);

	window->display();
}

sf::Vector2f Game::getRandomPos()
{
	sf::Vector2f posToReturn;

	posToReturn = sf::Vector2f((float)(rand() % window->getSize().x), (float)(rand() % window->getSize().y));

	return posToReturn;
}
