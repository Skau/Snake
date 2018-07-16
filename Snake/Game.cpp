#include "Game.h"
#include <memory>
#include <string>
#include <iostream>
#include <time.h>
#include "Snake.h"
#include "Button.h"
#include "Dot.h"

Game::Game() : difficulty{ 1 }, gameState{ GameState::MENU }, lastScore{ 0 }
{
	srand((int)time(NULL));

	window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 800), "Snake");
	window->setFramerateLimit(120);

	dotImage = std::make_unique<sf::Image>(sf::Image());

	if (!dotImage->loadFromFile("images/redImage.png"))
	{
		std::cerr << "Could not load red image!\n";
	}

	createButtons();

	createTexts();
}

void Game::init()
{
	score = 0;

	snake = std::make_shared<Snake>();

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	mainLoop();
}

void Game::createButtons()
{
	incrementDifficultyButton = std::make_shared<Button>(sf::Vector2f(((float)window->getSize().x / 2) + 200, ((float)window->getSize().y / 2) - 250), 200.f, 100.f, "Difficulty ++");

	incrementDifficultyButton->setOnClick([&]() {
		if (difficulty < 9)
			difficulty++;
	});

	allButtons.push_back(incrementDifficultyButton);

	decrementDifficultyButton = std::make_shared<Button>(sf::Vector2f(((float)window->getSize().x / 2) - 200, ((float)window->getSize().y / 2) - 250), 200.f, 100.f, "Difficulty --");

	decrementDifficultyButton->setOnClick([&]()
	{
		if (difficulty > 1)
			difficulty--;
	});

	allButtons.push_back(decrementDifficultyButton);

	exitButton = std::make_shared<Button>(sf::Vector2f(((float)window->getSize().x / 2), ((float)window->getSize().y / 2) + 230), 200.f, 100.f, "Quit game");

	exitButton->setOnClick([&]()
	{
		window->close();
	});

	allButtons.push_back(exitButton);
}

void Game::createTexts()
{
	font = std::make_unique<sf::Font>();

	if (!font->loadFromFile("fonts/arial.ttf"))
	{
		std::cerr << "Could not load font!\n";
	}
	difficultyText = std::make_unique<sf::Text>();
	difficultyText->setFont(*font);
	difficultyText->setCharacterSize(30);
	difficultyText->setPosition(sf::Vector2f(10, 10));

	scoreText = std::make_unique<sf::Text>();
	scoreText->setFont(*font);
	scoreText->setCharacterSize(70);
	scoreText->setPosition(sf::Vector2f(670, 700));

	lastScoreText = std::make_unique<sf::Text>();
	lastScoreText->setFont(*font);
	lastScoreText->setCharacterSize(30);
	lastScoreText->setPosition(sf::Vector2f(370, 740));

	resetText = std::make_unique<sf::Text>();
	resetText->setFont(*font);
	resetText->setCharacterSize(30);
	resetText->setPosition(sf::Vector2f(10, 740));
	resetText->setString("Press 'R' to reset game");
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

		if (event.type == sf::Event::MouseButtonPressed)
		{
			for (auto& button : allButtons)
			{
				if (button->getIsHovered())
				{
					button->onClick();
				}
			}
		}

		if (gameState == GameState::MENU)
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
			if (gameState == GameState::MENU)
			{
				gameState = GameState::INGAME;
			}
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
		switch (gameState)
		{
		case GameState::MENU:
			menuTick();
			break;
		case GameState::INGAME:
			inGameTick(timePerFrame.asSeconds());
			break;
		default:
			break;
		}
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

	if (gameState == GameState::MENU)
	{
		difficultyText->setString("Current difficulty level (1-9):   " + std::to_string(difficulty) + "    Spacebar to start game");
		window->draw(*difficultyText);

		lastScoreText->setString("Last score: " + std::to_string(lastScore));
		window->draw(*lastScoreText);

		for (auto& button : allButtons)
		{
			button->render(*window);
		}
	}

	window->draw(*resetText);

	window->display();
}

void Game::inGameTick(float deltaTime)
{
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

	if (dot.get())
	{
		if (snake->getHeadNode().getColBox().getGlobalBounds().intersects(dot->getColBox().getGlobalBounds()))
		{
			dot.reset();
			snake->spawnNewNode();
			score += difficulty;
		}
	}
	snake->tick(timePerFrame.asSeconds());
}

void Game::menuTick()
{
	auto pos = sf::Mouse::getPosition(*window);
	for (auto& button : allButtons)
	{
		button->Tick(pos);
	}
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

void Game::startGame()
{
	gameState = GameState::INGAME;
}

void Game::resetGame()
{
	if (score > 0)
	{
		lastScore = score;
	}

	snake.reset();
	if (dot.get())
	{
		dot.reset();
	}
	gameState = GameState::MENU;
	init();
}

void Game::exitGame()
{
	window->close();
}
