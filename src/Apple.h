#pragma once
#include "Entity.h"

class Apple : public Entity
{
	public:
		Apple(int x, int y) : Entity(x, y) {}
		Apple() : Entity(0, 0) {}

};

