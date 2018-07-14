#include "Snake.h"
#include "SFML/Graphics.hpp"
#include <iostream>

Snake::Snake()
{
	image = std::make_unique<sf::Image>();

	if (!image->loadFromFile("images/blueImage.png"))
	{
		std::cerr << "Could not load blue image!\n";
	}

	headNode = spawnNewNode();
}

void Snake::handleEvents(sf::Event& e)
{
	if (e.type == sf::Event::KeyPressed)
	{
		switch (e.key.code)
		{
		case sf::Keyboard::Up:
			direction = Direction::UP;
			isMoving = true;
			break;
		case sf::Keyboard::Down:
			direction = Direction::DOWN;
			isMoving = true;
			break;
		case sf::Keyboard::Left:
			direction = Direction::LEFT;
			isMoving = true;
			break;
		case sf::Keyboard::Right:
			direction = Direction::RIGHT;
			isMoving = true; 
			break;
		default:
			break;
		}
	}
}

void Snake::tick(float deltaTime)
{
	if (isMoving)
		move(deltaTime);
}


std::shared_ptr<Node> Snake::spawnNewNode()
{
	sf::Vector2f pos;

	if (nodes.size())
	{
		auto n = nodes.front();
		if (n.get())
		{
			switch (n->lastDirection)
			{
			case Direction::UP:
			{
				pos = sf::Vector2f(n->pos.x, n->pos.y - 24);
				break;
			}
			case Direction::DOWN:
			{
				pos = sf::Vector2f(n->pos.x, n->pos.y + 24);
				break;
			}
			case Direction::LEFT:
			{
				pos = sf::Vector2f(n->pos.x - 24, n->pos.y);
				break;
			}
			case Direction::RIGHT:
			{
				pos = sf::Vector2f(n->pos.x + 24, n->pos.y);
				break;
			}
			default:
				break;
			}
		}
		else
		{
			pos = sf::Vector2f(400, 400);
		}
	}
	else
	{
		pos = sf::Vector2f(400, 400);
	}

	std::shared_ptr<Node> newNode = std::make_shared<Node>(*image, pos);
	if (nodes.size())
	{
		newNode->setNodeInFront(nodes.back());
	}
		nodes.push_back(newNode);

	return newNode;
}

void Snake::move(float deltaTime)
{

	switch (direction)
	{
	case Direction::UP:
		headNode->lastPos = headNode->pos + sf::Vector2f(0, 24);
		headNode->pos.y -= vel * deltaTime;
		break;
	case Direction::DOWN:
		headNode->lastPos = headNode->pos - sf::Vector2f(0, 24);
		headNode->pos.y += vel * deltaTime;
		break;
	case Direction::LEFT:
		headNode->lastPos = headNode->pos + sf::Vector2f(24, 0);
		headNode->pos.x -= vel * deltaTime;
		break;
	case Direction::RIGHT:
		headNode->lastPos = headNode->pos - sf::Vector2f(24, 0);
		headNode->pos.x += vel * deltaTime;
		break;
	default:
		break;
	}

	if (headNode->pos.x <= 0)
	{
		headNode->pos.x += vel;
	}
	if(headNode->pos.y <= 0)
	{
		headNode->pos.y += vel;
	}
	if (headNode->pos.x >= 800)
	{
		headNode->pos.x -= vel;
	}
	if (headNode->pos.y >= 800)
	{
		headNode->pos.y -= vel;
	}

	headNode->lastDirection = direction;
	headNode->direction = direction;
	headNode->getSprite().setPosition(headNode->pos);
	headNode->getColBox().setPosition(headNode->pos);

	for (auto& node : nodes)
	{
		if (node->getNodeInFront())
		{
			switch (node->lastDirection)
			{
			case Direction::UP:
				node->lastPos = node->pos + sf::Vector2f(0, 24);
				break;
			case Direction::DOWN:
				node->lastPos = node->pos - sf::Vector2f(0, 24);
				break;
			case Direction::LEFT:
				node->lastPos = node->pos + sf::Vector2f(24, 0);
				break;
			case Direction::RIGHT:
				node->lastPos = node->pos - sf::Vector2f(24, 0);
				break;
			default:
				break;
			}
			node->pos = node->getNodeInFront()->lastPos;
			node->getSprite().setPosition(node->pos);
			node->getColBox().setPosition(node->pos);
		}
	}
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
