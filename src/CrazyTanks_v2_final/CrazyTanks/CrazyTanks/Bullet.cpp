#include "Bullet.h"
#include "Wall.h"

Bullet::Bullet(std::chrono::time_point<std::chrono::system_clock> time)
{
	_lastAction = time.time_since_epoch().count();
}

void Bullet::action(int x, int y, GameObject* gameData[][GameEngine::MAP_WIDTH])
{
	_rightNow = std::chrono::system_clock::now().time_since_epoch().count();

	if (_rightNow - _lastAction > _speed * 10000000)
	{
		_lastAction = _rightNow;

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
	}
}

int Bullet::getOwner()
{
	return _owner;
}

void Bullet::setOwner(int owner)
{
	_owner = owner;
}

int Bullet::getTask()
{
	return _task;
}

void Bullet::setTask(int task)
{
	_task = task;
}

void Bullet::move(int x, int y, int xx, int yy, GameObject* gameData[][GameEngine::MAP_WIDTH], char _playerDirection)
{
	if (gameData[x][y]->getObjectType() == _engine->bullet)
	{
		int step_x = xx, step_y = yy;

		// if the target cell is not occupied by something
		if (checkIsValidAction(step_x, step_y, gameData))
		{
			std::swap(gameData[x][y], gameData[step_x][step_y]);
		}
		else
		{
			// destroy a projectile
			GameObject * unit = new GameObject;
			unit->setObjectType(_engine->field);
			gameData[x][y] = unit;
		}
	}
}

bool Bullet::checkIsValidAction(int x, int y, GameObject* gameData[][GameEngine::MAP_WIDTH])
{
	if (x > GameEngine::MAP_HEIGHT - 1 || x < 0) // overstepping
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
	else if (gameData[x][y]->getObjectType() == _engine->wall) // various obstacles
	{
		Wall* wall = static_cast<Wall*>(gameData[x][y]);

		if (!wall->hitWall())
		{
			GameObject * unit = new GameObject;
			unit->setObjectType(_engine->field);
			gameData[x][y] = unit;
		}

		return false;
	}
	else if (gameData[x][y]->getObjectType() == _engine->enemy)
	{
		if (_owner == GameEngine::player)
		{
			GameObject * unit = new GameObject;
			unit->setObjectType(_engine->field);
			gameData[x][y] = unit;

			_engine->setDestroyedTanks(_engine->getDestroyedTanks() + 1);
		}

		return false;
	}
	else if (gameData[x][y]->getObjectType() == _engine->bullet)
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
		if (_owner != _engine->player)
		{
			_engine->setGold—aptured(true);
			return false;
		}
		return false;
	}
	else
	{
		return true;
	}
}

