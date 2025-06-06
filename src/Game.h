#pragma once
#include <vector>
#include "Snake.h"
#include "Apple.h"
#include <random>
#include <string>
#include <SFML/Graphics.hpp>

enum class CellType
{
	EMPTY = 0,
	SNAKE = 1,
	APPLE = 2
};

class Game
{
	public:
		Game();
		std::string startGame();
		void updateBoard();
		void drawBoardSFML();
		void spawnApple();
		bool checkCollision();

		void updateDirection(sf::Event event);

		bool checkApple();

	private:	
		const int boardHeight = 15;
		const int boardWidth = 18;
		const int tileSize = 48;
		std::vector<std::vector<CellType>> board;
		Snake snake;
		Apple apple;
		std::mt19937 rng;
		std::uniform_int_distribution<int> xDist;
		std::uniform_int_distribution<int> yDist;
		int score = 0;
		sf::RenderWindow window;
		sf::Font font;
		sf::Text gameText;

};
