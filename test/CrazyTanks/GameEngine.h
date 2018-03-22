#pragma once

#include <string>
#include <chrono>
#include "GameObject.h"

class Pawn;

class GameEngine
{
private:
	GameEngine() {};
	~GameEngine();

	static GameEngine* instance_;

	// system("cls") is flashes 
	static void clearScreen();

	// Tanks should not be placed closer than 2 sectors to each other
	static bool checkNearness(int row, int column);

	// User input events
	static void playerKeyInput();

	// to determine whether the button is released
	static bool _bRelease;

	static bool getKeyState();
	static void setKeyState(bool state);

	static int _lifeRemains;

	static int _destroyedTanks;
	static int _totalTanks;
	static bool _gold—aptured;

	static const int _taskRange = 4;

	static std::chrono::time_point<std::chrono::system_clock> _start, _now;

	// Player
	static Pawn* _playerUnit;

public:

	// pattern Singleton
	static GameEngine* getInstance();

	// game map size
	static const int MAP_HEIGHT = 20;
	static const int MAP_WIDTH = 50;

	std::chrono::time_point<std::chrono::system_clock> getNowTime();
	void setNowTime(std::chrono::time_point<std::chrono::system_clock> time);


	// Fill game data at first start
	static void init();

	// Draws the contents of an array
	static void draw();

	// Called in the main thread, updates the world state and accepts user input
	static void tick();

	// Get a random number with probability, for example 0.75
	static bool getRandomWithProbability(double probability);

	// Get a random number in range
	static int getRandomInRange(int min, int max);

	// Get the game time in the format 00:00:00
	static std::string getElapsedTime();

	enum gameObjects { field, wall, enemy, bullet, player, border, gold };
	enum tasks { forward, backward, left, right, fire };

	static int getLifeRemains();
	static void setLifeRemains(int value);
	static int getDestroyedTanks();
	static void setDestroyedTanks(int value);
	static int getTotalTanks();

	static bool isGold—aptured();
	static void setGold—aptured(bool value);

	static int getTaskRange();

	static Pawn* getPlayer();

	// 2D gaming data array
	static GameObject* gameData[MAP_HEIGHT][MAP_WIDTH];
};

