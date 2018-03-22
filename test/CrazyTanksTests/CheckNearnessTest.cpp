#include "pch.h"
#include "gmock/gmock.h"

#include <random>

#include "GameObject.h"
#include "Wall.h"
#include "Pawn.h"

// game map size
static const int MAP_HEIGHT = 20;
static const int MAP_WIDTH  = 20;

enum gameObjects { field, wall, enemy, bullet, player, border, gold };

// 2D gaming data array
GameObject* gameData[MAP_HEIGHT][MAP_WIDTH];

bool getRandomWithProbability(double probability)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 100);

	return dis(gen) % 100 < (probability * 100);
}

bool checkNearness(int row, int column)
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

void init()
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
				if (getRandomWithProbability(0.15))
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
			if (getRandomWithProbability(0.2))
			{
				if (checkNearness(i, j))
				{
					Pawn* unit = new Pawn(std::chrono::system_clock::now());
					unit->setObjectType(enemy);
					gameData[i][j] = unit;
				}
			}
		}
	}
}

void draw()
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

				std::wcout << "w";
			}
			else if (gameData[i][j]->getObjectType() == enemy)
			{
				std::wcout << "e";
			}
			else if (gameData[i][j]->getObjectType() == field)
			{
				std::wcout << " ";
			}
		}
		std::wcout << std::endl;
	}
}

TEST(CrazyTanks, CheckNearness)
{
	init();

	draw();

	EXPECT_TRUE(checkNearness(1, 1));
}