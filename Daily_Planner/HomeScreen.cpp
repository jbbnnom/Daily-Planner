#include "HomeScreen.h"

using namespace std;

// 메인 화면 출력
void HomeScreen::drawScreen(int& mode)
{
	system("mode con: cols=69 lines=40");

	int keyInput, posY = 22;
	auto now = chrono::system_clock::now();
	auto now_time = chrono::floor<chrono::days>(now);
	auto ymd = chrono::year_month_day{ now_time };

	for (int i = 0; i < 2; ++i) {
		HANDLE buffer = hBuffer[i];

		SetConsoleTitleA("Daily Planner");
		setConsoleSize(buffer, 69, 40);
		clearBuffer(buffer);
		SetConsoleCursorPosition(buffer, { 0, 0 });

		drawTitle(buffer);
		writeBuffer(buffer, 24, 18, "Today is " + to_string(static_cast<int>(ymd.year())) +
			"-" + to_string(static_cast<unsigned>(ymd.month())) +
			"-" + to_string(static_cast<unsigned>(ymd.day())));

		writeBuffer(buffer, 19, 20, "■〓〓〓〓〓〓〓〓〓〓〓〓〓■");
		writeBuffer(buffer, 26, 22, "Enter new to-do");
		writeBuffer(buffer, 26, 22 + 2, "Watch Calendar");
		writeBuffer(buffer, 26, 22 + 4, "Exit");
		writeBuffer(buffer, 19, 28, "■〓〓〓〓〓〓〓〓〓〓〓〓〓■");
	}


	while (true) {
		HANDLE buffer = hBuffer[!active];

		SetConsoleActiveScreenBuffer(buffer);


		for (int y = 22; y <= 26; y += 2) {
			writeBuffer(buffer, 22, y, "  ");
		}

		writeBuffer(buffer, 22, posY, "▶");

		if (_kbhit()) {
			keyInput = _getch();
			if (keyInput == ARROW) {
				keyInput = _getch();
				if (keyInput == UP && posY > 22) {
					posY -= 2;
				}
				else if (keyInput == DOWN && posY < 26) {
					posY += 2;
				}
			}
			else if (keyInput == ENTER) {
				if (posY == 22) {	
					mode = 1;
					break;
				}
				else if (posY == 24) {

				}
				else if (posY == 26) {
					exit(0);
				}
			}
		}

		swapBuffer();
		Sleep(16);
	}
}

void HomeScreen::drawTitle(HANDLE handle)
{
	string s;

	s += "\n  ########     ###    #### ##       ##    ##\n";
	s += "  ##     ##   ## ##    ##  ##        ##  ## \n";
	s += "  ##     ##  ##   ##   ##  ##         ####  \n";
	s += "  ##     ## ##     ##  ##  ##          ##   \n";
	s += "  ##     ## #########  ##  ##          ##   \n";
	s += "  ##     ## ##     ##  ##  ##          ##   \n";
	s += "  ########  ##     ## #### ########    ##   \n\n";

	s += "  ########  ##          ###    ##    ## ##    ## ######## ######## \n";
	s += "  ##     ## ##         ## ##   ###   ## ###   ## ##       ##     ##\n";
	s += "  ##     ## ##        ##   ##  ####  ## ####  ## ##       ##     ##\n";
	s += "  ########  ##       ##     ## ## ## ## ## ## ## ######   ######## \n";
	s += "  ##        ##       ######### ##  #### ##  #### ##       ##   ##  \n";
	s += "  ##        ##       ##     ## ##   ### ##   ### ##       ##    ## \n";
	s += "  ##        ######## ##     ## ##    ## ##    ## ######## ##     ##\n\n\n";

	writeBuffer(handle, 0, 0, s);
}
