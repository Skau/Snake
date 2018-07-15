#include "Button.h"
#include <iostream>

Button::Button(sf::Vector2f pos, float width, float height, std::string text) : isHovered{ false }
{
	box = std::make_unique<sf::RectangleShape>(sf::Vector2f(width,height));

	box->setFillColor(defaultColor);
	box->setOrigin(sf::Vector2f(width / 2, height / 2));
	box->setPosition(pos);
	box->setOutlineColor(sf::Color::White);
	box->setOutlineThickness(2.f);

	font = std::make_unique<sf::Font>();
	if (!font->loadFromFile("fonts/arial.ttf"))
	{
		std::cerr << "Failed to load button font!\n";
	}

	buttonText = std::make_unique<sf::Text>(text, *font, 32);
	sf::FloatRect textRect = buttonText->getLocalBounds();
	buttonText->setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
	buttonText->setPosition(box->getPosition());
}

void Button::Tick()
{
	if (isHovered)
	{
		box->setFillColor(hoveredColor);
	}
	else
	{
		box->setFillColor(defaultColor);
	}
}

void Button::render(sf::RenderWindow & window)
{
	if (!isHidden)
	{
		window.draw(*box);
		window.draw(*buttonText);
	}
}

bool Button::checkHover(sf::Vector2i mousePos)
{
	if (box->getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
	{
		isHovered = true;
	}
	else
	{
		isHovered = false;
	}

	return isHovered;
}

void Button::onClick()
{
	std::cout << "Button clicked!\n";
	
}