#pragma once

#include "GameObject.h"
#include "GameEngine.h"
#include <chrono>

class Bullet : public GameObject
{
private:

	float _speed = 0.25;
	long long _lastAction, _rightNow;

	void move(int x, int y, int xx, int yy, GameObject * gameData[][GameEngine::MAP_WIDTH], char _playerDirection = 'U');

	bool checkIsValidAction(int x, int y, GameObject * gameData[][GameEngine::MAP_WIDTH]);

	int _owner, _task;

	const GameEngine* _engine = GameEngine::getInstance();

public:

	Bullet(std::chrono::time_point<std::chrono::system_clock> time);

	void action(int x, int y, GameObject* gameData[][GameEngine::MAP_WIDTH]);

	int getOwner();
	void setOwner(int owner);

	int getTask();
	void setTask(int task);

}; 