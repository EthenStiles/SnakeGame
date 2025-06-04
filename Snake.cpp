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
	const Segment& head = segments[0];
	int newX = head.x;
	int newY = head.y;

	switch (direction) 
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
