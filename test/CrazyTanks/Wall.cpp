#include "Wall.h"

bool Wall::hitWall()
{
	lifes--;
	state = damaged;

	if (lifes > 0) 
		return true;
	else
		return false;
}

int Wall::getWallState()
{
	return state;
}
