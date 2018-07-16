#pragma once
#include "SFML/Graphics.hpp"

class Entity;
class Snake;
class Dot;
class Button;

enum class GameState
{
	MENU,
	INGAME
};

class Game
{
public:
	Game();
	~Game()=default;

	void init();

	int difficulty;
private:
	void createButtons();
	void createTexts();

	void mainLoop();
	void handleEvents();
	void mainTick(); 
	void render();

	void inGameTick(float deltaTime);
	void menuTick();

	sf::Vector2f getRandomPos();

	void startGame();
	void resetGame();
	void exitGame();

	int score;
	int lastScore;

	GameState gameState;

	sf::Clock frameClock;
	const sf::Time timePerFrame = sf::seconds(1.0f / 60.f);
	sf::Time timeSinceLastUpdate;

	std::unique_ptr<sf::Image> snakeImage;
	std::unique_ptr<sf::Image> dotImage;
	std::unique_ptr<sf::RenderWindow> window;

	std::shared_ptr<Snake> snake;
	std::shared_ptr<Dot> dot;

	std::unique_ptr<sf::Font> font;
	std::unique_ptr<sf::Text> difficultyText;
	std::unique_ptr<sf::Text> scoreText;
	std::unique_ptr<sf::Text> lastScoreText;
	std::unique_ptr<sf::Text> resetText;

	std::vector<std::shared_ptr<Button>> allButtons;
	std::shared_ptr<Button> exitButton;
	std::shared_ptr<Button> incrementDifficultyButton;
	std::shared_ptr<Button> decrementDifficultyButton;
};

