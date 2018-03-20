#pragma once

#include "GameObject.h"
#include <chrono>
#include "GlobalVars.h"
#include "GameEngine.h"

class Enemy : public GameObject
{
public:

	Enemy(std::chrono::time_point<std::chrono::system_clock> time);

	int task;
	float speed;
	int moveDistance;
	bool isTaskFinish = true;
	long long lastAction, rightNow;

	void action(int x, int y, GameObject * gameData[][MAP_WIDTH]);

	void move(int x, int y, int xx, int yy, GameObject * gameData[][MAP_WIDTH], char _playerDirection = 'U');

	void fire(int x, int y, GameObject * gameData[][MAP_WIDTH]);

	bool checkDistance(int distance);

	bool checkIsValidAction(int x, int y, GameObject * gameData[][MAP_WIDTH]);
};

