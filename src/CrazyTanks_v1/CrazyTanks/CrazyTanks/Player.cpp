#include "Player.h"
#include "Bullet.h"
#include "Field.h"
#include <chrono>

void Player::move(int x, int y, int xx, int yy, GameObject * gameData[][MAP_WIDTH], char _playerDirection)
{
	if (gameData[x][y]->object_type == player)
	{
		int step_x = xx, step_y = yy;

		if (checkIsValidPlayerAction(step_x, step_y, gameData))
		{
			static_cast<Player*>(gameData[x][y])->pos_w = step_y;
			static_cast<Player*>(gameData[x][y])->pos_h = step_x;
			static_cast<Player*>(gameData[x][y])->playerDirection = _playerDirection;

			pos_w = step_y;
			pos_h = step_x;
			playerDirection = _playerDirection;

			std::swap(gameData[x][y], gameData[step_x][step_y]);
		}
	}
}

void Player::fire(GameObject * gameData[][MAP_WIDTH])
{
	int step_x = pos_w, step_y = pos_h;
	int direction;

	if (playerDirection == 'U') { step_y = pos_h - 1; direction = t_forward; }
	else if (playerDirection == 'D') { step_y = pos_h + 1; direction = t_backward; }
	else if (playerDirection == 'L') { step_x = pos_w - 1; direction = t_left; }
	else if (playerDirection == 'R') { step_x = pos_w + 1; direction = t_right; }

	if (checkIsValidAction(step_y, step_x, gameData))
	{
		Bullet * unit = new Bullet(std::chrono::system_clock::now());
		unit->object_type = bullet;
		unit->owner = player;
		unit->task = direction;
		unit->speed = 0.25;
		gameData[step_y][step_x] = unit;
	}
}

bool Player::checkIsValidAction(int x, int y, GameObject * gameData[][MAP_WIDTH])
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
	else if (gameData[x][y]->object_type == bullet)
	{
		return false;
	}
	else if (gameData[x][y]->object_type == player)
	{
		engine->lifeRemains--;
		return false;
	}
	else
	{
		return true;
	}
}

bool Player::checkIsValidPlayerAction(int x, int y, GameObject * gameData[][MAP_WIDTH])
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
		Field * unit = new Field;
		unit->object_type = field;
		gameData[x][y] = unit;

		engine->destroyedTanks++;

		return true;
	}
	return true;
}

