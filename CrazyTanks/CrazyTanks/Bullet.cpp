#include "Bullet.h"
#include "Field.h"

Bullet::Bullet(std::chrono::time_point<std::chrono::system_clock> time)
{
	lastAction = time.time_since_epoch().count();
}

void Bullet::action(int x, int y, GameObject * gameData[][MAP_WIDTH])
{
	rightNow = std::chrono::system_clock::now().time_since_epoch().count();

	if (rightNow - lastAction > speed * 10000000)
	{
		lastAction = rightNow;

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
	}
}

void Bullet::move(int x, int y, int xx, int yy, GameObject * gameData[][MAP_WIDTH], char _playerDirection)
{
	if (gameData[x][y]->object_type == bullet)
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
			Field * unit = new Field;
			unit->object_type = field;
			gameData[x][y] = unit;
		}
	}
}

bool Bullet::checkIsValidAction(int x, int y, GameObject * gameData[][MAP_WIDTH])
{
	if (x > MAP_HEIGHT - 1 || x < 0) // overstepping
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
	else if (gameData[x][y]->object_type == wall) // various obstacles
	{
		return false;
	}
	else if (gameData[x][y]->object_type == enemy)
	{
		if (owner == player)
		{
			Field * unit = new Field;
			unit->object_type = field;
			gameData[x][y] = unit;

			GameEngine::destroyedTanks++;
		}

		return false;
	}
	else if (gameData[x][y]->object_type == bullet)
	{
		return false;
	}
	else if (gameData[x][y]->object_type == player)
	{
		GameEngine::lifeRemains--;
		return false;
	}
	else
	{
		return true;
	}
}

