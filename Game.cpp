#include "game.h"
#include <iostream>
#include <chrono>
#include <conio.h>
#include <thread>

Game::Game() 
	: board(boardWidth, std::vector<CellType>(boardHeight, CellType::EMPTY)),
	  snake(),
	  apple(),
	  rng(std::chrono::steady_clock::now().time_since_epoch().count()),
	  xDist(0, boardWidth - 1),
	  yDist(0, boardHeight - 1)

{
	spawnApple();
}

std::string Game::startGame() 
{
	using clock = std::chrono::steady_clock;
	auto lastMoveTime = clock::now();
	
	while(true)
	{
		updateDirection();
		auto now = clock::now();
		if (now - lastMoveTime >= std::chrono::milliseconds(250))
		{
			snake.move();
			lastMoveTime = now;
			system("cls");
			if (checkCollision())
			{
				std::string result = "";
				
				std::string outcome = (score == ((boardHeight * boardWidth) - snake.getInitialSize())) ? "Won!" : "Lost!";
								
				result.append("You " + outcome + "\n");
				result.append("Snake Size: " + std::to_string(snake.getSegments().size()) + "\n");
				result.append("Score: " + std::to_string(score) + "\n");
				
				return result;
			}

			checkApple();
			drawBoard();
			
		}
	}
}

static char getCellSymbol(CellType c) 
{
	switch (c) 
	{
		case CellType::EMPTY:
			return '.';
		case CellType::SNAKE:
			return 'O';
		case CellType::APPLE:
			return '@';
		default: return '?';

	}
}

void Game::drawBoard()
{
	
	for (int y = 0; y < boardHeight; y++)
	{
		for (int x = 0; x < boardWidth; x++)
		{
			board[x][y] = CellType::EMPTY;
		}
	}
	
	board[apple.x][apple.y] = CellType::APPLE;
	
	for (const Segment& seg : snake.getSegments()) {
		
		board[seg.x][seg.y] = CellType::SNAKE;  // or any value to represent the snake
	}
	
	for (int y = 0; y < boardHeight; y++)
	{
		for (int x = 0; x < boardWidth; x++)
		{
			std::cout << getCellSymbol(board[x][y]);
		}

		std::cout << std::endl;
	}

	std::cout << "Snake Size: " << snake.getSegments().size() << std::endl;
	std::cout << "Apples Eaten: " << score << std::endl;
}

void Game::spawnApple() 
{
	int x = xDist(rng);
	int y = yDist(rng);

	while (board[x][y] != CellType::EMPTY) 
	{
		x = xDist(rng);
		y = yDist(rng);
	}
	
	apple.setPosition(x, y);
	board[x][y] = CellType::APPLE;
}

bool Game::checkCollision() 
{
	const Segment& head = snake.getSegments()[0];
	int x = head.x;
	int y = head.y;

	if (x < 0 || x >= boardWidth || y < 0 || y >= boardHeight) 
	{
		return true;
	}

	const auto& segments = snake.getSegments();
	for (int i = 1 ; i < segments.size(); i++) 
	{
		if (segments[i].x == x && segments[i].y == y) { return true; }
	}

	return false;
}

void Game::updateDirection() {
	if (_kbhit()) { // if a key has been pressed
		char ch = _getch();
		switch (ch) {
		case 'w':
			snake.setDirection(Direction::UP);
			break;
		case 'a':
			snake.setDirection(Direction::LEFT);
			break;
		case 's':
			snake.setDirection(Direction::DOWN);
			break;
		case 'd':
			snake.setDirection(Direction::RIGHT);
			break;
		}
	}
}

bool Game::checkApple()
{
	const Segment& head = snake.getSegments()[0];

	if (head.x == apple.x && head.y == apple.y) 
	{
		spawnApple();
		score++;
		snake.grow();
		return true;
	}

	return false;
}