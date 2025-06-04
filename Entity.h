#pragma once
class Entity
{
	public:
		Entity(int x = 0, int y = 0) : x(x), y(y){}
		void setPosition(int newX, int newY) { x = newX, y = newY; }
		int x;
		int y;
};

