#pragma once

#include <string>
#include <chrono>

class Player;

class GameEngine
{
public:

	// Fill game data at first start
	void init();

	// Draws the contents of an array
	void draw();

	// system("cls") is flashes 
	void clearScreen();

	// Called in the main thread, updates the world state and accepts user input
	void tick();

	// Tanks should not be placed closer than 2 sectors to each other
	bool checkNearness(int row, int column);

	// Get a random number with probability, for example 0.75
	bool getRandomWithProbability(double probability);

	// Get a random number in range
	int getRandomInRange(int min, int max);

	// User input events
	void playerKeyInput();

	// Get the game time in the format 00:00:00
	std::string getElapsedTime();

	enum gameObjects { field, wall, enemy, bullet, player, border };
	enum tasks { t_forward, t_backward, t_left, t_right, t_fire };

	// to determine whether the button is released
	bool bRelease = true;

	static int lifeRemains;
	static int destroyedTanks;
	int totalTanks = 0;

	std::chrono::time_point<std::chrono::system_clock> start, now;

	// Player
	Player * playerUnit;

};

