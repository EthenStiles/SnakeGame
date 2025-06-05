#pragma once
#include "Entity.h"
#include <vector>

enum class Direction 
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

class Segment : public Entity
{
	public:
		Segment(int x, int y) : Entity(x, y) {}
		
};

class Snake
{
	public:
		Snake();
		const std::vector<Segment>& getSegments() const { return segments; }
		void move();
		void setDirection(Direction d);
		Direction getDirection() { return direction; }
		void grow() { ateApple = true; }
		int getInitialSize() { return initialSize; }
	private:
		std::vector<Segment> segments;
		int initialSize = 3;
		Direction direction;
		Direction nextDirection;
		bool ateApple = false;
};

