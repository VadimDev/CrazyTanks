#include "Pawn.h"
#include "Bullet.h"

Pawn::Pawn()
{
}

Pawn::Pawn(std::chrono::time_point<std::chrono::system_clock> time)
{
	_lastAction = time.time_since_epoch().count();
}

void Pawn::setPlayerDirection(char direction)
{
	_playerDirection = direction;
}

char Pawn::getPlayerDirection()
{
	return _playerDirection;
}

void Pawn::setPosition(int height, int width)
{
	_pos_h = height;
	_pos_w = width;
}

int Pawn::getHeight()
{
	return _pos_h;
}

int Pawn::getWidth()
{
	return _pos_w;
}

void Pawn::setTask(int task)
{
	_task = task;
}

void Pawn::move(int x, int y, int xx, int yy, GameObject * gameData[][GameEngine::MAP_WIDTH], char playerDirection)
{
	int step_x = xx, step_y = yy;

	if (gameData[x][y]->getObjectType() == _engine->enemy)
	{
		// If there is still a margin for this task
		if (checkDistance(_moveDistance))
		{
			// if the target cell is not occupied by something
			if (checkIsValidAction(step_x, step_y, gameData, _engine->enemy))
			{
				std::swap(gameData[x][y], gameData[step_x][step_y]);
			}
			else
			{
				// complete the task ahead of time
				_isTaskFinish = true;
				_task = _engine->getRandomInRange(0, _engine->getTaskRange());
			}
		}
	}
	else if (gameData[x][y]->getObjectType() == _engine->player)
	{
		if (checkIsValidAction(step_x, step_y, gameData, _engine->player))
		{
			static_cast<Pawn*>(gameData[x][y])->setPosition(step_x, step_y);
			static_cast<Pawn*>(gameData[x][y])->setPlayerDirection(_playerDirection);

			_pos_w = step_y;
			_pos_h = step_x;
			_playerDirection = playerDirection;

			std::swap(gameData[x][y], gameData[step_x][step_y]);
		}
	}
}

void Pawn::fire(GameObject * gameData[][GameEngine::MAP_WIDTH])
{
	int step_x = _pos_w, step_y = _pos_h;
	int direction;

	if (_playerDirection == 'U') { step_y = _pos_h - 1; direction = _engine->forward; }
	else if (_playerDirection == 'D') { step_y = _pos_h + 1; direction = _engine->backward; }
	else if (_playerDirection == 'L') { step_x = _pos_w - 1; direction = _engine->left; }
	else if (_playerDirection == 'R') { step_x = _pos_w + 1; direction = _engine->right; }

	if (checkIsValidAction(step_y, step_x, gameData, _engine->player))
	{
		Bullet * unit = new Bullet(std::chrono::system_clock::now());
		unit->setObjectType(_engine->bullet);
		unit->setOwner(_engine->player);
		unit->setTask(direction);
		gameData[step_y][step_x] = unit;
	}
}

void Pawn::fire(int x, int y, GameObject * gameData[][GameEngine::MAP_WIDTH])
{
	int direction = _engine->getRandomInRange(0, 3); // choose direction
	int step_x = x, step_y = y;

	if (direction == _engine->forward) step_x = x - 1;
	else if (direction == _engine->backward) step_x = x + 1;
	else if (direction == _engine->left) step_y = y - 1;
	else if (direction == _engine->right) step_y = y + 1;

	// if shooting in this direction is possible
	if (checkIsValidAction(step_x, step_y, gameData, _engine->enemy))
	{
		// create a new object on the map
		Bullet * unit = new Bullet(std::chrono::system_clock::now());
		unit->setObjectType(_engine->bullet);
		unit->setTask(direction);
		gameData[step_x][step_y] = unit;
	}
	else
	{
		// complete the task ahead of schedule
		_task = _engine->getRandomInRange(0, _engine->getTaskRange());
	}
}

void Pawn::action(int x, int y, GameObject * gameData[][GameEngine::MAP_WIDTH])
{
	_rightNow = std::chrono::system_clock::now().time_since_epoch().count();

	if (_rightNow - _lastAction > _speed * 10000000)
	{
		_lastAction = _rightNow;

		if (_task == _engine->forward && _isTaskFinish || _task == _engine->backward && _isTaskFinish)
		{
			_moveDistance = _engine->getRandomInRange(0, GameEngine::MAP_HEIGHT - 1);
			_isTaskFinish = false;
		}
		else if (_task == _engine->left && _isTaskFinish || _task == _engine->right && _isTaskFinish)
		{
			_moveDistance = _engine->getRandomInRange(0, GameEngine::MAP_WIDTH - 1);
			_isTaskFinish = false;
		}

		if (_task == _engine->forward)
		{
			move(x, y, x - 1, y, gameData);
		}
		else if (_task == _engine->backward)
		{
			move(x, y, x + 1, y, gameData);
		}
		else if (_task == _engine->left)
		{
			move(x, y, x, y - 1, gameData);
		}
		else if (_task == _engine->right)
		{
			move(x, y, x, y + 1, gameData);
		}
		else if (_task == _engine->fire)
		{
			fire(x, y, gameData);
		}
	}
}

bool Pawn::checkDistance(int distance)
{
	if (_moveDistance > 0)
	{
		_moveDistance--;
		return true;
	}
	else
	{
		_isTaskFinish = true;
		_task = _engine->getRandomInRange(0, _engine->getTaskRange());
		return false;
	}
}

bool Pawn::checkIsValidAction(int x, int y, GameObject * gameData[][GameEngine::MAP_WIDTH], int ownerType)
{
	if (x > GameEngine::MAP_HEIGHT - 1 || x < 0)
	{
		return false;
	}
	else if (y > GameEngine::MAP_WIDTH - 1 || y < 0)
	{
		return false;
	}
	else if (gameData[x][y]->getObjectType() == _engine->border)
	{
		return false;
	}
	else if (gameData[x][y]->getObjectType() == _engine->wall)
	{
		return false;
	}
	else if (ownerType == _engine->enemy) // specific requirements
	{
		if (gameData[x][y]->getObjectType() == _engine->enemy)
		{
			return false;
		}
		else if (gameData[x][y]->getObjectType() == _engine->player)
		{
			_engine->setLifeRemains(_engine->getLifeRemains() - 1); 
			return false;
		}
		else if (gameData[x][y]->getObjectType() == _engine->gold)
		{
			_engine->setGoldÑaptured(true);
			return false;
		}
	}
	else if (ownerType == _engine->player)
	{
		if (gameData[x][y]->getObjectType() == _engine->enemy) // tank destruction by ram
		{
			GameObject * unit = new GameObject;
			unit->setObjectType(_engine->field);
			gameData[x][y] = unit;

			_engine->setDestroyedTanks(_engine->getDestroyedTanks() + 1);

			return true;
		}
		else if (gameData[x][y]->getObjectType() == _engine->gold)
		{
			return false;
		}
	}
	return true;
}
