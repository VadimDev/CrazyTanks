#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "GameEngine.h"
#include "GlobalVars.h"

using namespace std;

int main()
{
	// encoding settings and console window size
	_setmode(_fileno(stdout), _O_U16TEXT);

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 800, 600, TRUE);

	engine->init();

	// main game cycle
	while (true)
	{
		if (engine->totalTanks == 0)
		{
			system("cls");

			wcout << "Congratulations you won! Press ENTER to exit.";

			cin.get();

			break;
		}
		else if (engine->lifeRemains == 0)
		{
			system("cls");

			wcout << "You lose :( Press ENTER to exit.";

			cin.get();

			break;
		}
		else
		{
			Sleep(50);

			engine->now = chrono::system_clock::now();

			engine->tick();
		}
	}

	return 0;
}