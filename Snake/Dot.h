#pragma once
#include "SFML/Graphics.hpp"
class Dot
{
public:
	Dot(sf::Image& image, sf::Vector2f pos);
	Dot()=default;
	~Dot()=default;

	virtual void render(sf::RenderWindow& window);

	sf::RectangleShape& getColBox() { return *colBox; }

private:
	sf::Vector2f pos;

	std::unique_ptr<sf::Sprite> sprite;
	std::unique_ptr<sf::Texture> texture;
	std::unique_ptr<sf::RectangleShape> colBox;

};

