#include "Enemy.h"
#include "Bullet.h"

Enemy::Enemy(std::chrono::time_point<std::chrono::system_clock> time)
{
	lastAction = time.time_since_epoch().count();
}

void Enemy::action(int x, int y, GameObject * gameData[][MAP_WIDTH])
{
	rightNow = std::chrono::system_clock::now().time_since_epoch().count();

	if (rightNow - lastAction > speed * 10000000)
	{
		lastAction = rightNow;

		if (task == t_forward && isTaskFinish && object_type != bullet || task == t_backward && isTaskFinish && object_type != bullet)
		{
			moveDistance = engine->getRandomInRange(0, MAP_HEIGHT - 1);
			isTaskFinish = false;
		}
		else if (task == t_left && isTaskFinish && object_type != bullet || task == t_right && isTaskFinish && object_type != bullet)
		{
			moveDistance = engine->getRandomInRange(0, MAP_WIDTH - 1);
			isTaskFinish = false;
		}

		if (task == t_forward)
		{
			move(x, y, x - 1, y, gameData);
		}
		else if (task == t_backward)
		{
			move(x, y, x + 1, y, gameData);
		}
		else if (task == t_left)
		{
			move(x, y, x, y - 1, gameData);
		}
		else if (task == t_right)
		{
			move(x, y, x, y + 1, gameData);
		}
		else if (task == t_fire)
		{
			fire(x, y, gameData);
		}
	}
}

void Enemy::move(int x, int y, int xx, int yy, GameObject * gameData[][MAP_WIDTH], char _playerDirection)
{
	if (gameData[x][y]->object_type == enemy)
	{
		// If there is still a margin for this task
		if (checkDistance(moveDistance))
		{
			int step_x = xx, step_y = yy;

			// if the target cell is not occupied by something
			if (checkIsValidAction(step_x, step_y, gameData))
			{
				std::swap(gameData[x][y], gameData[step_x][step_y]);
			}
			else
			{
				// complete the task ahead of time
				isTaskFinish = true;
				task = engine->getRandomInRange(0, taskRange);
			}
		}
	}
}

void Enemy::fire(int x, int y, GameObject * gameData[][MAP_WIDTH])
{
	int direction = engine->getRandomInRange(0, 3); // choose direction
	int step_x = x, step_y = y;

	if (direction == t_forward) step_x = x - 1;
	else if (direction == t_backward) step_x = x + 1;
	else if (direction == t_left) step_y = y - 1;
	else if (direction == t_right) step_y = y + 1;

	// if shooting in this direction is possible
	if (checkIsValidAction(step_x, step_y, gameData))
	{
		// create a new object on the map
		Bullet * unit = new Bullet(std::chrono::system_clock::now());
		unit->object_type = bullet;
		unit->task = direction;
		unit->speed = 0.25;
		gameData[step_x][step_y] = unit;
	}
	else
	{
		// complete the task ahead of schedule
		task = engine->getRandomInRange(0, taskRange);
	}
}

bool Enemy::checkDistance(int distance)
{
	if (moveDistance > 0)
	{
		moveDistance--;
		return true;
	}
	else
	{
		isTaskFinish = true;
		task = engine->getRandomInRange(0, taskRange);
		return false;
	}
}

bool Enemy::checkIsValidAction(int x, int y, GameObject * gameData[][MAP_WIDTH])
{
	if (x > MAP_HEIGHT - 1 || x < 0)
	{
		return false;
	}
	else if (y > MAP_WIDTH - 1 || y < 0)
	{
		return false;
	}
	else if (gameData[x][y]->object_type == border)
	{
		return false;
	}
	else if (gameData[x][y]->object_type == wall)
	{
		return false;
	}
	else if (gameData[x][y]->object_type == enemy)
	{
		return false;
	}
	else if (gameData[x][y]->object_type == player)
	{
		GameEngine::lifeRemains--;
		return false;
	}
	return true;
}


