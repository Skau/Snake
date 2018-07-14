#include "Dot.h"
#include <iostream>


Dot::Dot(sf::Image& image, sf::Vector2f pos)
{
	this->pos = pos;

	texture = std::make_unique<sf::Texture>(sf::Texture());
	texture->loadFromImage(image);

	sprite = std::make_unique<sf::Sprite>(sf::Sprite());
	sprite->setTexture(*texture);
	sprite->setOrigin((float)texture->getSize().x / 2, (float)texture->getSize().y / 2);
	sprite->setPosition(pos);

	colBox = std::make_unique<sf::RectangleShape>(sf::RectangleShape((sf::Vector2f((float)texture->getSize().x, (float)texture->getSize().y))));
	colBox->setOrigin((float)texture->getSize().x / 2, (float)texture->getSize().y / 2);
	colBox->setPosition(sprite->getPosition());
	colBox->setOutlineThickness(1.5f);
	colBox->setFillColor(sf::Color::Transparent);
	colBox->setOutlineColor(sf::Color::White);
}

void Dot::render(sf::RenderWindow & window)
{
	window.draw(*sprite);
	window.draw(*colBox);
}