#include "Game.h"
#include <memory>
#include <string>
#include <iostream>
#include <time.h>
#include "Snake.h"
#include "Dot.h"

Game::Game() : difficulty{ 1 }
{
	srand((int)time(NULL));

	window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 800), "Snake");
	window->setFramerateLimit(120);

	dotImage = std::make_unique<sf::Image>(sf::Image());

	if (!dotImage->loadFromFile("images/redImage.png"))
	{
		std::cerr << "Could not load red image!\n";
	}

	font = std::make_unique<sf::Font>();

	if (!font->loadFromFile("fonts/arial.ttf"))
	{
		std::cerr << "Could not load font!\n";
	}
	difficultyText = std::make_unique<sf::Text>();
	difficultyText->setFont(*font);
	difficultyText->setCharacterSize(20);
	difficultyText->setPosition(sf::Vector2f(10, 10));

	scoreText = std::make_unique<sf::Text>();
	scoreText->setFont(*font);
	scoreText->setCharacterSize(70);
	scoreText->setPosition(sf::Vector2f(670, 700));

	resetText = std::make_unique<sf::Text>();
	resetText->setFont(*font);
	resetText->setCharacterSize(30);
	resetText->setPosition(sf::Vector2f(10, 750));
	resetText->setString("Press 'R' to reset game");
}

void Game::init()
{
	score = 0;

	snake = std::make_shared<Snake>();

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

		if (!isPlaying)
		{
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Key::Num1:
					difficulty = 1;
					break;
				case sf::Keyboard::Key::Num2:
					difficulty = 2;
					break;
				case sf::Keyboard::Key::Num3:
					difficulty = 3;
					break;
				case sf::Keyboard::Key::Num4:
					difficulty = 4;
					break;
				case sf::Keyboard::Key::Num5:
					difficulty = 5;
					break;
				case sf::Keyboard::Key::Num6:
					difficulty = 6;
					break;
				case sf::Keyboard::Key::Num7:
					difficulty = 7;
					break;
				case sf::Keyboard::Key::Num8:
					difficulty = 8;
					break;
				case sf::Keyboard::Key::Num9:
					difficulty = 9;
					break;
				default:
					break;
				}
			}
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
		{
			if (!isPlaying)
				isPlaying = true;
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
		{
			resetGame();
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

		if (!snake->getHasSetDifficulty())
		{
			snake->setDifficulty(difficulty);
		}

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
			score+= difficulty;
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

	if (!isPlaying)
	{
		difficultyText->setString("Current difficulty level: " + std::to_string(difficulty) + " (To change, press numbers 1 - 9)");
		window->draw(*difficultyText);
	}

	window->draw(*resetText);
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
	if (dot.get())
		dot.reset();
	isPlaying = false;
	init();
}
