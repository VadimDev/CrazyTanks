#pragma once

#include "GameObject.h"
#include "GlobalVars.h"
#include <chrono>

class Bullet : public GameObject
{
public:

	Bullet(std::chrono::time_point<std::chrono::system_clock> time);

	int task;
	float speed;
	long long lastAction, rightNow;
	int owner;

	void action(int x, int y, GameObject * gameData[][MAP_WIDTH]);

	void move(int x, int y, int xx, int yy, GameObject * gameData[][MAP_WIDTH], char _playerDirection = 'U');

	bool checkIsValidAction(int x, int y, GameObject * gameData[][MAP_WIDTH]);
};

