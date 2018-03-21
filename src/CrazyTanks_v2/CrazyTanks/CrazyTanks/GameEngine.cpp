#include <windows.h>
#include <iostream>
#include <random>

#include "GameEngine.h"
#include "GameObject.h"
#include "Wall.h"
#include "Bullet.h"
#include "Pawn.h"

int GameEngine::_lifeRemains     = 3;
int GameEngine::_destroyedTanks  = 0;
int GameEngine::_totalTanks      = 0;
bool GameEngine::_goldÑaptured = false;

bool GameEngine::_bRelease = true;

GameEngine* GameEngine::instance_ = nullptr;
Pawn* GameEngine::_playerUnit = new Pawn;

std::chrono::time_point<std::chrono::system_clock> GameEngine::_start = std::chrono::system_clock::now();
std::chrono::time_point<std::chrono::system_clock> GameEngine::_now = GameEngine::_start;

GameObject* GameEngine::gameData[MAP_HEIGHT][MAP_WIDTH];

GameEngine::~GameEngine()
{
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			delete gameData[i][j];
		}
	}
}

GameEngine * GameEngine::getInstance()
{
	return (!instance_) ? instance_ = new GameEngine : instance_;
}

std::chrono::time_point<std::chrono::system_clock> GameEngine::getNowTime()
{
	return _now;
}

void GameEngine::setNowTime(std::chrono::time_point<std::chrono::system_clock> time)
{
	_now = time;
}

void GameEngine::init()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// borders
			if (i == 0 || i == MAP_HEIGHT - 1 || j == 0 || j == MAP_WIDTH - 1)
			{
				GameObject * unit = new GameObject;
				unit->setObjectType(border);
				gameData[i][j] = unit;
			}
			else
			{
				if (getRandomWithProbability(0.1))
				{
					Wall * unit = new Wall;
					unit->setObjectType(wall);
					gameData[i][j] = unit;
				}
				else
				{
					GameObject * unit = new GameObject;
					unit->setObjectType(field);
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
					Pawn* unit = new Pawn(std::chrono::system_clock::now());
					unit->setObjectType(enemy);
					// setup initial tasks
					unit->setTask(getRandomInRange(0, _taskRange));

					gameData[i][j] = unit;
					_totalTanks++;
				}
			}
		}
	}

	// spawn stronghold and gold
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j <= 2; j++)
		{
			if (i == 1 && j == 1)
			{
				GameObject* treasure = new GameObject;
				treasure->setObjectType(gold);
				gameData[MAP_HEIGHT - 2][MAP_WIDTH / 2] = treasure; // in the middle
			}
			else
			{
				Wall* stronghold = new Wall;
				stronghold->setObjectType(wall);
				gameData[MAP_HEIGHT - 2 - 1 + i][(MAP_WIDTH / 2) - 1 + j] = stronghold;
			}
		}
	}

	// spawn player
	_playerUnit->setObjectType(player);

	// on the last line in the middle, the direction is chosen randomly
	int direction = getRandomInRange(0, 1);

	for (int x = MAP_WIDTH / 2; x > 0 && x < MAP_WIDTH;)
	{
		if (gameData[MAP_HEIGHT - 2][x]->getObjectType() == field)
		{
			_playerUnit->setPosition(MAP_HEIGHT - 2, x);
			gameData[MAP_HEIGHT - 2][x] = _playerUnit;

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
			if (gameData[i][j]->getObjectType() == border)
			{
				std::wcout << "#";
			}
			else if (gameData[i][j]->getObjectType() == wall)
			{
				Wall* wall = static_cast<Wall*>(gameData[i][j]);

				if (wall->getWallState() == Wall::intact)
					std::wcout << L"\u2593";
				else
					std::wcout << L"\u2591";
			}
			else if (gameData[i][j]->getObjectType() == enemy)
			{
				std::wcout << L"\u263C";
			}
			else if (gameData[i][j]->getObjectType() == bullet)
			{
				std::wcout << L"+";
			}
			else if (gameData[i][j]->getObjectType() == field)
			{
				std::wcout << " ";
			}
			else if (gameData[i][j]->getObjectType() == gold)
			{
				std::wcout << "$";
			}
			else if (gameData[i][j]->getObjectType() == player)
			{
				if (static_cast<Pawn*>(gameData[i][j])->getPlayerDirection() == 'U')
				{
					std::wcout << L"\u25B2";
				}
				else if (static_cast<Pawn*>(gameData[i][j])->getPlayerDirection() == 'D')
				{
					std::wcout << L"\u25BC";
				}
				else if (static_cast<Pawn*>(gameData[i][j])->getPlayerDirection() == 'L')
				{
					std::wcout << L"\u25C4";
				}
				else if (static_cast<Pawn*>(gameData[i][j])->getPlayerDirection() == 'R')
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

	std::string time_and_life = "| GAME TIME : " + getElapsedTime() + "         " + "LIFE REMAINS: " + std::to_string(_lifeRemains);

	for (int i = time_and_life.length(); i < MAP_WIDTH; i++)
	{
		if (i != MAP_WIDTH - 1)
			time_and_life += " ";
		else
			time_and_life += "|";
	}

	std::string emptyLine = " ";

	for (int i = emptyLine.length(); i < MAP_WIDTH; i++)
	{
		if (i != MAP_WIDTH - 1)
			emptyLine += " ";
		else
			emptyLine += "|";
	}

	std::string tanksStat = "| TOTAL TANKS : " + std::to_string(_totalTanks) + "          " + "DESTROYED TANKS: " + std::to_string(_destroyedTanks);

	for (int i = tanksStat.length(); i < MAP_WIDTH; i++)
	{
		if (i != MAP_WIDTH - 1)
			tanksStat += " ";
		else
			tanksStat += "|";
	}

	std::wcout << std::wstring(header_horizontal.begin(), header_horizontal.end()) << std::endl
		<< std::wstring(time_and_life.begin(), time_and_life.end()) << std::endl
		<< std::wstring(emptyLine.begin(), emptyLine.end()) << std::endl
		<< std::wstring(tanksStat.begin(), tanksStat.end()) << std::endl
		<< std::wstring(header_horizontal.begin(), header_horizontal.end()) << std::endl;

	// reload world
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (gameData[i][j]->getObjectType() == enemy)
			{
				static_cast<Pawn*>(gameData[i][j])->action(i, j, gameData);
			}
			else if (gameData[i][j]->getObjectType() == bullet)
			{
				static_cast<Bullet*>(gameData[i][j])->action(i, j, gameData);
			}
		}
	}

	draw();
}

bool GameEngine::checkNearness(int row, int column)
{
	if (gameData[row][column]->getObjectType() == border || gameData[row][column]->getObjectType() == wall)
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
				if (gameData[i][j]->getObjectType() == enemy)
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
		if (_bRelease)
		{
			_bRelease = false;

			_playerUnit->setPlayerDirection('U');

			_playerUnit->move(_playerUnit->getHeight(), _playerUnit->getWidth(), _playerUnit->getHeight() - 1, _playerUnit->getWidth(), gameData, 'U');
		}
	}
	else if (GetAsyncKeyState(VK_DOWN) < 0)
	{
		if (_bRelease)
		{
			_bRelease = false;

			_playerUnit->setPlayerDirection('D');

			_playerUnit->move(_playerUnit->getHeight(), _playerUnit->getWidth(), _playerUnit->getHeight() + 1, _playerUnit->getWidth(), gameData, 'D');
		}
	}
	else if (GetAsyncKeyState(VK_LEFT) < 0)
	{
		if (_bRelease)
		{
			_bRelease = false;

			_playerUnit->setPlayerDirection('L');

			_playerUnit->move(_playerUnit->getHeight(), _playerUnit->getWidth(), _playerUnit->getHeight(), _playerUnit->getWidth() - 1, gameData, 'L');
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT) < 0)
	{
		if (_bRelease)
		{
			_bRelease = false;

			_playerUnit->setPlayerDirection('R');

			_playerUnit->move(_playerUnit->getHeight(), _playerUnit->getWidth(), _playerUnit->getHeight(), _playerUnit->getWidth() + 1, gameData, 'R');
		}
	}
	else if (GetAsyncKeyState(VK_SPACE) < 0)
	{
		if (_bRelease)
		{
			_bRelease = false;

			static_cast<Pawn*>(gameData[_playerUnit->getHeight()][_playerUnit->getWidth()])->fire(gameData);
		}
	}
	else
	{
		_bRelease = true;
	}
}

bool GameEngine::getKeyState()
{
	return _bRelease;
}

void GameEngine::setKeyState(bool state)
{
	_bRelease = state;
}

std::string GameEngine::getElapsedTime()
{
	auto milliseconds = (_now - _start).count();

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

int GameEngine::getLifeRemains()
{
	return _lifeRemains;
}

void GameEngine::setLifeRemains(int value)
{
	_lifeRemains = value;
}

int GameEngine::getDestroyedTanks()
{
	return _destroyedTanks;
}

void GameEngine::setDestroyedTanks(int value)
{
	_destroyedTanks = value;
}

int GameEngine::getTotalTanks()
{
	return _totalTanks;
}

bool GameEngine::isGoldÑaptured()
{
	return _goldÑaptured;
}

void GameEngine::setGoldÑaptured(bool value)
{
	_goldÑaptured = value;
}

int GameEngine::getTaskRange()
{
	return _taskRange;
}

Pawn* GameEngine::getPlayer()
{
	return _playerUnit;
}
