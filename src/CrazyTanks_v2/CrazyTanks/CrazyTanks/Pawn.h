#pragma once

#include <chrono>

#include "GameObject.h"
#include "GameEngine.h"

class Pawn : public GameObject
{
	bool checkDistance(int distance);

	bool checkIsValidAction(int x, int y, GameObject* gameData[][GameEngine::MAP_WIDTH], int ownerType);

	int _moveDistance;
	bool _isTaskFinish = true;

	long long _lastAction, _rightNow;

	float _speed = 0.5; // 1 cell per second

	int _task;

	int _pos_h, _pos_w;

	char _playerDirection = 'U';

	const GameEngine* _engine = GameEngine::getInstance();

public:
	Pawn();
	Pawn(std::chrono::time_point<std::chrono::system_clock> time);

	void setPlayerDirection(char direction);
	char getPlayerDirection();

	void setPosition(int height, int width);
	int getHeight();
	int getWidth();

	void setTask(int task);

	void move(int x, int y, int xx, int yy, GameObject* gameData[][GameEngine::MAP_WIDTH], char playerDirection = 'U');

	void fire(GameObject* gameData[][GameEngine::MAP_WIDTH]);

	void fire(int x, int y, GameObject * gameData[][GameEngine::MAP_WIDTH]);

	void action(int x, int y, GameObject * gameData[][GameEngine::MAP_WIDTH]);
};

