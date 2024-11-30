#include "HomeScreen.h"

using namespace std;

// 메인 화면 출력
void HomeScreen::drawScreen(int& mode)
{
	system("mode con: cols=69 lines=40");	// 기본 창 크기 설정

	int keyInput, posY = 22;

	// 오늘 날짜를 받아와 저장
	auto now = chrono::system_clock::now();
	auto now_time = chrono::floor<chrono::days>(now);
	auto ymd = chrono::year_month_day{ now_time };

	// 더블 버퍼링을 위해 준비한 2개의 버퍼 설정 적용
	for (int i = 0; i < 2; ++i) {
		HANDLE buffer = hBuffer[i];

		SetConsoleTitleA("Daily Planner");
		setConsoleSize(buffer, 69, 40);
		clearBuffer(buffer);
		SetConsoleCursorPosition(buffer, { 0, 0 });

		drawTitle(buffer);
		
		// 오늘의 할 일(구현 예정) 및 메뉴 출력
		writeBuffer(buffer, 24, 18, "Today is " + to_string(static_cast<int>(ymd.year())) +
			"-" + to_string(static_cast<unsigned>(ymd.month())) +
			"-" + to_string(static_cast<unsigned>(ymd.day())));

		writeBuffer(buffer, 19, 20, "■〓〓〓〓〓〓〓〓〓〓〓〓〓■");
		writeBuffer(buffer, 26, 22, "Enter new to-do");
		writeBuffer(buffer, 26, 22 + 2, "Watch Calendar");
		writeBuffer(buffer, 26, 22 + 4, "Exit");
		writeBuffer(buffer, 19, 28, "■〓〓〓〓〓〓〓〓〓〓〓〓〓■");
	}

	// 키보드 입력이 있을 때마다 2개의 버퍼가 계속해서 바뀌게 되어 깜빡거림 현상 해결
	while (true) {
		HANDLE buffer = hBuffer[!active];

		SetConsoleActiveScreenBuffer(buffer);	// 현재 출력되는 화면을 비활성화된 버퍼로 변경

		for (int y = 22; y <= 26; y += 2) {
			writeBuffer(buffer, 22, y, "  ");
		}

		writeBuffer(buffer, 22, posY, "▶");

		// 키보드 입력
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

// 타이틀 출력
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
