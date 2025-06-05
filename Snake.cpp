#include "Snake.h"

Snake::Snake()
{
	int startX = 5;
	int startY = 5;
	direction = Direction::RIGHT;
	
	for (int i = 0; i < initialSize; i++)
	{
		segments.emplace_back(startX - i, startY);
	}
};

void Snake::move() 
{
	direction = nextDirection;
	const Segment& head = segments[0];
	int newX = head.x;
	int newY = head.y;

	switch (nextDirection) 
	{
		case Direction::UP:
			newY--;
			break;
		case Direction::RIGHT:
			newX++;
			break;
		case Direction::DOWN:
			newY++;
			break;
		case Direction::LEFT:
			newX--;
			break;
	}

	segments.insert(segments.begin(), Segment(newX, newY));
	if (!ateApple) 
	{
		segments.pop_back();
	}
	ateApple = false;
	
}

void Snake::setDirection(Direction d) {
	// Prevent reversing direction
	if ((direction == Direction::UP && d == Direction::DOWN) ||
		(direction == Direction::DOWN && d == Direction::UP) ||
		(direction == Direction::LEFT && d == Direction::RIGHT) ||
		(direction == Direction::RIGHT && d == Direction::LEFT)) {
		return;
	}
	nextDirection = d;  // Queue the next direction
}
