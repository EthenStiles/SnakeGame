#include "game.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>

Game::Game()
	: board(boardWidth, std::vector<CellType>(boardHeight, CellType::EMPTY)),
	  snake(),
	  apple(),
	  rng(std::chrono::steady_clock::now().time_since_epoch().count()),
	  xDist(0, boardWidth - 1),
	  yDist(0, boardHeight - 1)

{
	spawnApple();

	if (!font.loadFromFile("assets/arial.ttf")) {
		std::cout << "couldn't load font" << std::endl;
	}

	gameText.setFont(font);
	gameText.setCharacterSize(18); // pixels
	gameText.setFillColor(sf::Color::White);
	gameText.setPosition(5, 5); // top-left corner
}

std::string Game::startGame() 
{
	using clock = std::chrono::steady_clock;
	auto lastMoveTime = clock::now();
	std::string result = "";
	
	window.create(sf::VideoMode(boardWidth * tileSize, boardHeight * tileSize), "Snake Game");
	
	while(window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed)
				updateDirection(event);
		}

		auto now = clock::now();
		if (now - lastMoveTime >= std::chrono::milliseconds(250))
		{
			snake.move();
			lastMoveTime = now;
			system("cls");
			if (checkCollision())
			{
				std::string outcome = (score == ((boardHeight * boardWidth) - snake.getInitialSize())) ? "Won!" : "Lost!";
								
				result.append("You " + outcome + "\n");
				result.append("Snake Size: " + std::to_string(snake.getSegments().size()) + "\n");
				result.append("Score: " + std::to_string(score) + "\n");
				
				break;
			}

			checkApple();
			updateBoard();
			drawBoardSFML();
		}
	}

	return result;
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

void Game::updateBoard()
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

	std::cout << "Snake Size: " << snake.getSegments().size() << std::endl;
	std::cout << "Apples Eaten: " << score << std::endl;
}

void Game::drawBoardSFML()
{
	window.clear();

	std::unordered_map<int, int> segmentIndexMap;
	const auto& segments = snake.getSegments();
	int max_brightness = 255;
	int min_brightness = 75;

	for (int i = 0; i < segments.size(); ++i) {
		int key = segments[i].x + segments[i].y * boardWidth;
		segmentIndexMap[key] = i;
	}
	
	for (int y = 0; y < boardHeight; y++)
	{
		for (int x = 0; x < boardWidth; x++)
		{
			sf::RectangleShape tile(sf::Vector2f(tileSize - 2, tileSize - 2));
			tile.setPosition(x * tileSize, y * tileSize);

			switch (board[x][y]) 
			{
				case CellType::EMPTY: 
				{
					tile.setFillColor(sf::Color(25, 25, 25));
					break;
				}
					
				case CellType::SNAKE: {
					int key = x + y * boardWidth;
					auto it = segmentIndexMap.find(key);

					if (it != segmentIndexMap.end()) {
						int idx = it->second;
						int total = static_cast<int>(segments.size());
						int brightness = max_brightness - ((max_brightness - min_brightness) * idx / std::max(1, total - 1));
						tile.setFillColor(sf::Color(0, brightness, 0));
					}
					else {
						tile.setFillColor(sf::Color::Magenta); // Shouldn't happen: debug color
					}
					break;
				}

				case CellType::APPLE: 
				{
					tile.setFillColor(sf::Color::Red);
					break;
				}	
			}

			window.draw(tile);
		}
	}

	std::string infoStr =
		"Score: " + std::to_string(score) +
		" | Snake Size: " + std::to_string(snake.getSegments().size());

	gameText.setString(infoStr);
	window.draw(gameText);

	window.display();
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

void Game::updateDirection(sf::Event event) {
	// if a key has been pressed
	switch (event.key.code) 
	{
		case sf::Keyboard::W:
			//prevent snake from going down if it is currently going up
			if (snake.getDirection() == Direction::DOWN) { return; }
			snake.setDirection(Direction::UP);
			break;
		case sf::Keyboard::A:
			//prevent snake from going left if it is currently going right
			if (snake.getDirection() == Direction::RIGHT) { return; }
			snake.setDirection(Direction::LEFT);
			break;
		case sf::Keyboard::S:
			//prevent snake from going down if it is currently going up
			if (snake.getDirection() == Direction::UP) { return; }
			snake.setDirection(Direction::DOWN);
			break;
		case sf::Keyboard::D:
			//prevent snake from going right if it is currently going left
			if (snake.getDirection() == Direction::LEFT) { return; }
			snake.setDirection(Direction::RIGHT);
			break;
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