#pragma once
#include "SFML/Graphics.hpp"

class Entity;
class Snake;
class Dot;

class Game
{
public:
	Game();
	~Game()=default;

	void init();

private:
	void mainLoop();
	void handleEvents();
	void mainTick();
	void render();

	sf::Vector2f getRandomPos();

	void resetGame();

	bool isPlaying;

	int score;

	sf::Clock frameClock;
	const sf::Time timePerFrame = sf::seconds(1.0f / 60.f);
	sf::Time timeSinceLastUpdate;

	std::unique_ptr<sf::Image> snakeImage;
	std::unique_ptr<sf::Image> dotImage;
	std::unique_ptr<sf::RenderWindow> window;

	std::shared_ptr<Snake> snake;
	std::shared_ptr<Dot> dot;

	std::unique_ptr<sf::Font> scoreFont;
	std::unique_ptr<sf::Text> scoreText;
};

