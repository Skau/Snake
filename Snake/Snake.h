#pragma once
#include "SFML/Graphics.hpp"

enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

struct Node
{
	sf::RectangleShape& getColBox() { return *colBox; }
	sf::Sprite& getSprite() { return *sprite; }
	sf::Texture& getTexture() { return *texture; }

	sf::Vector2f pos;
	sf::Vector2f lastPos;


	Node(sf::Image& image, sf::Vector2f pos)
	{
		this->pos = pos;
		lastPos = pos;
		texture = std::make_unique<sf::Texture>(sf::Texture());
		texture->loadFromImage(image);

		sprite = std::make_unique<sf::Sprite>(sf::Sprite());
		sprite->setTexture(*texture);
		sprite->setOrigin((float)texture->getSize().x / 2, (float)texture->getSize().y / 2);
		sprite->setPosition(pos);

		colBox = std::make_unique<sf::RectangleShape>(sf::RectangleShape((sf::Vector2f((float)texture->getSize().x, (float)texture->getSize().y))));
		colBox->setOrigin((float)texture->getSize().x / 2, (float)texture->getSize().y / 2);
		colBox->setPosition(sprite->getPosition());

		colBox->setFillColor(sf::Color::Transparent);
		colBox->setOutlineColor(sf::Color::White);
	}

private:
	std::unique_ptr<sf::Sprite> sprite;
	std::unique_ptr<sf::Texture> texture;
	std::unique_ptr<sf::RectangleShape> colBox;
};

class Snake
{
public:
	Snake();
	~Snake()=default;

	void handleEvents(sf::Event& e);

	void tick(float deltaTime);

	virtual void render(sf::RenderWindow& window);

	std::vector<sf::RectangleShape> getColBoxes();

	std::vector<std::shared_ptr<Node>>& getNodes() { return nodes; }

	Node& getHeadNode() { return *headNode; }

	std::shared_ptr<Node> spawnNewNode();

private:
	void move(float deltaTime);

	Direction direction;

	Direction lastDirection;

	bool isMoving;

	std::unique_ptr<sf::Image> image;

	std::vector<std::shared_ptr<Node>> nodes;

	std::shared_ptr<Node> headNode;

	const float vel = 1500;

	sf::Clock movementClock;
	sf::Time timeBetweenMovementElapsed;
	const sf::Time timeBetweenMovement = sf::milliseconds(50);
};

