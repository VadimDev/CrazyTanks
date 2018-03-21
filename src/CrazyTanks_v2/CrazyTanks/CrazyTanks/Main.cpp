using namespace std;

#include <windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "GameEngine.h"

int main()
{
	// encoding settings and console window size
	_setmode(_fileno(stdout), _O_U16TEXT);

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 800, 600, TRUE);

	//wcout << L"\u2593" << " " << L"\u2592" << " " << L"\u2591";
	//cin.get();

	GameEngine* engine = GameEngine::getInstance();

	engine->init();

	// main game cycle
	while (true)
	{
		if (engine->getTotalTanks() == 0)
		{
			system("cls");

			wcout << "Congratulations you won! Press ENTER to exit.";

			cin.get();

			break;
		}
		else if (engine->getLifeRemains() == 0 || engine->isGoldСaptured())
		{
			system("cls");

			wcout << "You lose :( Press ENTER to exit.";

			cin.get();

			break;
		}
		else
		{
			Sleep(50);

			engine->setNowTime(chrono::system_clock::now());

			engine->tick();
		}
	}

	return 0;
}