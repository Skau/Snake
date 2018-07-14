#include "Game.h"
#include <memory>
#include <string>
#include <iostream>
#include <time.h>
#include "Snake.h"
#include "Dot.h"

Game::Game()
{
	srand((int)time(NULL));

	window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 800), "Snake");
	window->setFramerateLimit(120);

	dotImage = std::make_unique<sf::Image>(sf::Image());

	if (!dotImage->loadFromFile("images/redImage.png"))
	{
		std::cerr << "Could not load red image!\n";
	}

	scoreFont = std::make_unique<sf::Font>();

	if (!scoreFont->loadFromFile("fonts/arial.ttf"))
	{
		std::cerr << "Could not load font!\n";
	}

	scoreText = std::make_unique<sf::Text>();
	scoreText->setFont(*scoreFont);
	scoreText->setCharacterSize(70);
	scoreText->setPosition(sf::Vector2f(700, 700));
}

void Game::init()
{
	score = 0;

	snake = std::make_shared<Snake>();

	dot = std::make_shared<Dot>(*dotImage, sf::Vector2f(200, 200));

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	mainLoop();
}

void Game::mainLoop()
{
	while (window->isOpen())
	{
		handleEvents();

		if(isPlaying)
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

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
		{
			isPlaying = !isPlaying;
		}

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

		for (int i = 3; i < snake->getNodes().size(); ++i)
		{
			if (snake->getHeadNode().getColBox().getGlobalBounds().intersects(snake->getNodes()[i]->getColBox().getGlobalBounds()))
			{
				resetGame();
			}
		}

		if (snake->getNodes().front()->getColBox().getGlobalBounds().intersects(dot->getColBox().getGlobalBounds()))
		{
			dot.reset();
			snake->spawnNewNode();
			score++;
		}

		snake->tick(timePerFrame.asSeconds());
	}

}

void Game::render()
{
	window->clear();

	snake->render(*window);

	if(dot.get())
	dot->render(*window);

	scoreText->setString(std::to_string(score));
	window->draw(*scoreText);
	window->display();
}

sf::Vector2f Game::getRandomPos()
{
	sf::Vector2f posToReturn;
	bool isIntersecting = true;

	while (isIntersecting)
	{
		posToReturn = sf::Vector2f((float)(rand() % window->getSize().x), (float)(rand() % window->getSize().y));

		for (auto box : snake->getColBoxes())
		{
			if (!(box.getGlobalBounds().contains(posToReturn)))
			{
				isIntersecting = false;
			}
		}
	}

	return posToReturn;
}

void Game::resetGame()
{
	snake.reset();
	isPlaying = false;
	init();
}
