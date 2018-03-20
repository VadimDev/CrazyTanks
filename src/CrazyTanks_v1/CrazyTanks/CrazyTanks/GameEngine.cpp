#include "GameEngine.h"
#include <windows.h>
#include "GlobalVars.h"
#include "GameObject.h"
#include "Border.h"
#include "Wall.h"
#include "Field.h"
#include "Enemy.h"
#include <iostream>
#include <random>
#include "Bullet.h"
#include "Player.h"

int GameEngine::lifeRemains = 3;
int GameEngine::destroyedTanks = 0;

void GameEngine::init()
{
	start = std::chrono::system_clock::now();

	playerUnit = new Player;

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// borders
			if (i == 0 || i == MAP_HEIGHT - 1 || j == 0 || j == MAP_WIDTH - 1)
			{
				Border * unit = new Border;
				unit->object_type = border;
				gameData[i][j] = unit;
			}
			else
			{
				if (getRandomWithProbability(0.1))
				{
					Wall * unit = new Wall;
					unit->object_type = wall;
					gameData[i][j] = unit;
				}
				else
				{
					Field * unit = new Field;
					unit->object_type = field;
					gameData[i][j] = unit;
				}
			}
		}
	}

	// spawn enemy tanks
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (getRandomWithProbability(0.01))
			{
				if (checkNearness(i, j))
				{
					Enemy * unit = new Enemy(std::chrono::system_clock::now());
					unit->object_type = enemy;
					// setup initial tasks
					unit->task = getRandomInRange(0, taskRange);
					unit->speed = 0.5; // 1 cell per second

					gameData[i][j] = unit;
					totalTanks++;
				}
			}
		}
	}

	// spawn player
	playerUnit->object_type = player;

	// on the last line in the middle, the direction is chosen randomly
	int direction = getRandomInRange(0, 1);

	for (int x = MAP_WIDTH / 2; x > 0 && x < MAP_WIDTH;)
	{
		if (gameData[MAP_HEIGHT - 2][x]->object_type == field)
		{
			playerUnit->pos_w = x;
			playerUnit->pos_h = MAP_HEIGHT - 2;

			gameData[MAP_HEIGHT - 2][x] = playerUnit;

			break;
		}

		if (direction == 0) x--;
		else x++;
	}
}

void GameEngine::draw()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (gameData[i][j]->object_type == border)
			{
				std::wcout << "#";
			}
			else if (gameData[i][j]->object_type == wall)
			{
				std::wcout << L"\u2592";
			}
			else if (gameData[i][j]->object_type == enemy)
			{
				std::wcout << L"\u263C";
			}
			else if (gameData[i][j]->object_type == bullet)
			{
				std::wcout << L"+";
			}
			else if (gameData[i][j]->object_type == field)
			{
				std::wcout << " ";
			}
			else if (gameData[i][j]->object_type == player)
			{
				if (static_cast<Player*>(gameData[i][j])->playerDirection == 'U')
				{
					std::wcout << L"\u25B2";
				}
				else if (static_cast<Player*>(gameData[i][j])->playerDirection == 'D')
				{
					std::wcout << L"\u25BC";
				}
				else if (static_cast<Player*>(gameData[i][j])->playerDirection == 'L')
				{
					std::wcout << L"\u25C4";
				}
				else if (static_cast<Player*>(gameData[i][j])->playerDirection == 'R')
				{
					std::wcout << L"\u25BA";
				}
			}
		}
		std::wcout << std::endl;
	}
}

void GameEngine::clearScreen()
{
	HANDLE hOut;
	COORD Position;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

void GameEngine::tick()
{
	clearScreen();

	playerKeyInput();

	std::string header_horizontal;

	for (int i = 0; i < MAP_WIDTH; i++)
	{
		if (i != 0 && i != MAP_WIDTH - 1)
			header_horizontal += "-";
		else
			header_horizontal += "+";
	}

	std::string time_line = "| GAME TIME : " + getElapsedTime();

	for (int i = time_line.length(); i < MAP_WIDTH; i++)
	{
		if (i != MAP_WIDTH - 1)
			time_line += " ";
		else
			time_line += "|";
	}

	std::string lifes = "| LIFE REMAINS: " + std::to_string(lifeRemains);

	for (int i = lifes.length(); i < MAP_WIDTH; i++)
	{
		if (i != MAP_WIDTH - 1)
			lifes += " ";
		else
			lifes += "|";
	}

	std::string desTanks = "| DESTROYED TANKS: " + std::to_string(destroyedTanks);

	for (int i = desTanks.length(); i < MAP_WIDTH; i++)
	{
		if (i != MAP_WIDTH - 1)
			desTanks += " ";
		else
			desTanks += "|";
	}

	std::string totTanks = "| TOTAL TANKS: " + std::to_string(totalTanks);

	for (int i = totTanks.length(); i < MAP_WIDTH; i++)
	{
		if (i != MAP_WIDTH - 1)
			totTanks += " ";
		else
			totTanks += "|";
	}

	std::wcout << std::wstring(header_horizontal.begin(), header_horizontal.end()) << std::endl 
		<< std::wstring(time_line.begin(), time_line.end()) << std::endl
		<< std::wstring(totTanks.begin(), totTanks.end()) << std::endl
		<< std::wstring(lifes.begin(), lifes.end()) << std::endl
		<< std::wstring(desTanks.begin(), desTanks.end()) << std::endl
		<< std::wstring(header_horizontal.begin(), header_horizontal.end()) << std::endl;

	// reload world
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (gameData[i][j]->object_type == enemy)
			{
				static_cast<Enemy*>(gameData[i][j])->action(i, j, gameData);
			}
			else if (gameData[i][j]->object_type == bullet)
			{
				static_cast<Bullet*>(gameData[i][j])->action(i, j, gameData);
			}
		}
	}

	draw();
}

bool GameEngine::checkNearness(int row, int column)
{
	if (gameData[row][column]->object_type == border || gameData[row][column]->object_type == wall)
	{
		return false;
	}
	else
	{
		int low_i, hight_i, low_j, hight_j;

		// row
		if (row - 2 < 0) low_i = 0;
		else low_i = row - 2;

		if (row + 2 > MAP_HEIGHT - 1) hight_i = MAP_HEIGHT - 1;
		else hight_i = row + 2;

		// column
		if (column - 2 < 0) low_j = 0;
		else low_j = column - 2;

		if (column + 2 > MAP_WIDTH - 1) hight_j = MAP_WIDTH - 1;
		else hight_j = column + 2;

		for (int i = low_i; i <= hight_i; i++)
		{
			for (int j = low_j; j <= hight_j; j++)
			{
				if (gameData[i][j]->object_type == enemy)
				{
					return false;
				}
			}
		}
		return true;
	}
}

bool GameEngine::getRandomWithProbability(double probability)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 100);

	return dis(gen) % 100 < (probability * 100);
}

int GameEngine::getRandomInRange(int min, int max)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<> uni(min, max);

	return uni(rng);
}

void GameEngine::playerKeyInput()
{
	if (GetAsyncKeyState(VK_UP) < 0)
	{
		if (bRelease)
		{
			bRelease = false;

			playerUnit->playerDirection = 'U';

			playerUnit->move(playerUnit->pos_h, playerUnit->pos_w, playerUnit->pos_h - 1, playerUnit->pos_w, gameData, 'U');
		}
	}
	else if (GetAsyncKeyState(VK_DOWN) < 0)
	{
		if (bRelease)
		{
			bRelease = false;

			playerUnit->playerDirection = 'D';

			playerUnit->move(playerUnit->pos_h, playerUnit->pos_w, playerUnit->pos_h + 1, playerUnit->pos_w, gameData, 'D');
		}
	}
	else if (GetAsyncKeyState(VK_LEFT) < 0)
	{
		if (bRelease)
		{
			bRelease = false;

			playerUnit->playerDirection = 'L';

			playerUnit->move(playerUnit->pos_h, playerUnit->pos_w, playerUnit->pos_h, playerUnit->pos_w - 1, gameData, 'L');
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT) < 0)
	{
		if (bRelease)
		{
			bRelease = false;

			playerUnit->playerDirection = 'R';

			playerUnit->move(playerUnit->pos_h, playerUnit->pos_w, playerUnit->pos_h, playerUnit->pos_w + 1, gameData, 'R');
		}
	}
	else if (GetAsyncKeyState(VK_SPACE) < 0)
	{
		if (bRelease)
		{
			bRelease = false;

			static_cast<Player*>(gameData[playerUnit->pos_h][playerUnit->pos_w])->fire(gameData);
		}
	}
	else
	{
		bRelease = true;
	}
}

std::string GameEngine::getElapsedTime()
{
	auto milliseconds = (now - start).count();

	int seconds = (int)(milliseconds / 10000000) % 60;
	int minutes = (int)((milliseconds / (10000000 * 60)) % 60);
	int hours = (int)((milliseconds / (10000000 * 60 * 60)) % 24);

	std::string h, m, s;

	if (hours < 10)
	{
		h = "0" + std::to_string(hours);
	}
	else {
		h = std::to_string(hours);
	}

	if (minutes < 10)
	{
		m = "0" + std::to_string(minutes);
	}
	else
	{
		m = std::to_string(minutes);
	}

	if (seconds < 10)
	{
		s = "0" + std::to_string(seconds);
	}
	else
	{
		s = std::to_string(seconds);
	}

	return h + ":" + m + ":" + s;
}
