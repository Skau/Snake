#pragma once
#include "SFML/Graphics.hpp"
class Button
{
public:
	Button() = default;
	Button(sf::Vector2f pos, float width, float height, std::string text);
	~Button() = default;

	void Tick();

	void render(sf::RenderWindow& window);

	sf::RectangleShape& getBox() { return *box; }

	bool checkHover(sf::Vector2i mousePos);

	void onClick();

	void setIsHidden(bool value) { isHidden = value; }

	bool getIsHidden() { return isHidden; }

private:
	std::unique_ptr<sf::RectangleShape> box;
	std::unique_ptr<sf::Font> font;
	std::unique_ptr<sf::Text> buttonText;
	const sf::Color defaultColor = sf::Color::Black;
	const sf::Color hoveredColor = sf::Color::Blue; 
	bool isHidden;
	bool isHovered;
};