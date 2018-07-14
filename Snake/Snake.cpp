#include "Snake.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

Snake::Snake()
{
	image = std::make_unique<sf::Image>();

	if (!image->loadFromFile("images/blueImage.png"))
	{
		std::cerr << "Could not load blue image!\n";
	}

	headNode = std::make_shared<Node>(*image, sf::Vector2f(400,400));
	nodes.push_back(headNode);
	nodes.emplace_back(std::make_shared<Node>(*image, sf::Vector2f(nodes.back()->pos.x - 24, 400)));
	nodes.emplace_back(std::make_shared<Node>(*image, sf::Vector2f(nodes.back()->pos.x - 24, 400)));
	nodes.emplace_back(std::make_shared<Node>(*image, sf::Vector2f(nodes.back()->pos.x - 24, 400)));
	nodes.emplace_back(std::make_shared<Node>(*image, sf::Vector2f(nodes.back()->pos.x - 24, 400)));

	timeBetweenMovementElapsed = sf::Time::Zero;

	direction = Direction::RIGHT;
	lastDirection = direction;
}

void Snake::handleEvents(sf::Event& e)
{
	if (e.type == sf::Event::KeyPressed)
	{
		switch (e.key.code)
		{
		case sf::Keyboard::Up:
			if (lastDirection != Direction::DOWN)
			{
				direction = Direction::UP;
			}
			break;
		case sf::Keyboard::Down:
			if (lastDirection != Direction::UP)
			{
				direction = Direction::DOWN;
			}
			break;
		case sf::Keyboard::Left:
			if (lastDirection != Direction::RIGHT)
			{
				direction = Direction::LEFT;
			}
			break;
		case sf::Keyboard::Right:
			if (lastDirection != Direction::LEFT)
			{
				direction = Direction::RIGHT;
			}
			break;
		default:
			break;
		}
	}
}

void Snake::tick(float deltaTime)
{
	timeBetweenMovementElapsed += movementClock.restart();

	if (timeBetweenMovementElapsed > timeBetweenMovement)
	{
		move(deltaTime);
	}
}


std::shared_ptr<Node> Snake::spawnNewNode()
{
	sf::Vector2f pos;

	auto direction = (nodes[nodes.size() - 1]->pos - nodes[nodes.size() - 2]->pos);
	pos = nodes[nodes.size() - 1]->pos + direction;

	std::shared_ptr<Node> newNode = std::make_shared<Node>(*image, pos);
	{

	return newNode;
}

void Snake::move(float deltaTime)
{
	headNode->lastPos = headNode->pos;
	
	switch (direction)
	{
	case Direction::UP:
		headNode->pos.y -= vel * deltaTime;
		break;
	case Direction::DOWN:
		headNode->pos.y += vel * deltaTime;
		break;
	case Direction::LEFT:
		headNode->pos.x -= vel * deltaTime;
		break;
	case Direction::RIGHT:
		headNode->pos.x += vel * deltaTime;
		break;
	default:
		break;
	}
	 
	if (headNode->pos.x < 0)
	{
		headNode->pos.x = 800;
	}
	if (headNode->pos.y < 0)
	{
		headNode->pos.y = 800;
	}
	if (headNode->pos.x > 800)
	{
		headNode->pos.x = 0;
	}
	if (headNode->pos.y > 800)
	{
		headNode->pos.y = 0;
	}

	headNode->getSprite().setPosition(headNode->pos);
	headNode->getColBox().setPosition(headNode->pos);

	for (int i = 1; i < nodes.size(); ++i)
	{
		nodes[i]->lastPos = nodes[i]->pos;
		nodes[i]->pos = nodes[i - 1]->lastPos;
		nodes[i]->getSprite().setPosition(nodes[i]->pos);
		nodes[i]->getColBox().setPosition(nodes[i]->pos);
	}

	timeBetweenMovementElapsed = sf::Time::Zero;

	lastDirection = direction;
}

void Snake::render(sf::RenderWindow & window)
{
	for (auto& node : nodes)
	{
		window.draw(node->getSprite());
		window.draw(node->getColBox());
	}
}

std::vector<sf::RectangleShape> Snake::getColBoxes()
{
	std::vector<sf::RectangleShape> vectorToReturn;
	for (auto& node : nodes)
	{
		vectorToReturn.push_back(node->getColBox());
	}
	return vectorToReturn;
}
