#pragma once

#include "GameObject.h"
#include "GlobalVars.h"

class Player : public GameObject
{
public:

	int pos_h, pos_w;
	char playerDirection = 'U';

	void move(int x, int y, int xx, int yy, GameObject * gameData[][MAP_WIDTH], char _playerDirection = 'U');

	void fire(GameObject * gameData[][MAP_WIDTH]);

	bool checkIsValidAction(int x, int y, GameObject * gameData[][MAP_WIDTH]);

	bool checkIsValidPlayerAction(int x, int y, GameObject * gameData[][MAP_WIDTH]);
};

