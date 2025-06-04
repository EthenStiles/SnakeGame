#pragma once
#include <vector>
#include "Snake.h"
#include "Apple.h"
#include <random>
#include <string>

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
		void drawBoard();
		void spawnApple();
		bool checkCollision();

		void updateDirection();

		bool checkApple();

	private:	
		const int boardHeight = 10;
		const int boardWidth = 15;
		std::vector<std::vector<CellType>> board;
		Snake snake;
		Apple apple;
		std::mt19937 rng;
		std::uniform_int_distribution<int> xDist;
		std::uniform_int_distribution<int> yDist;
		int score = 0;
};
