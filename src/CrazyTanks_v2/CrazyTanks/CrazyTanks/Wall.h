#pragma once

#include "GameObject.h"

class Wall : public GameObject
{
	int lifes = 2;
	int state = intact;

public:

	enum states { damaged, intact };

	bool hitWall();
	int getWallState();


};

