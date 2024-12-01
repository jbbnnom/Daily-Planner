#include "LoadToDoScreen.h"

using namespace std;

/* To-Do 불러오기 창 */
void LoadToDoScreen::drawScreen(int& mode)
{
	system("mode con: cols=69 lines=40");	// 기본 창 크기 설정

	int keyInput, posY = 22;

	// 오늘 날짜를 받아와 저장
	auto now = chrono::system_clock::now();
	auto now_time = chrono::floor<chrono::days>(now);
	auto ymd = chrono::year_month_day{ now_time };

	// 더블 버퍼링을 위해 준비한 2개의 버퍼 설정 적용
	for (int i = 0; i < 2; i++) {
		HANDLE buffer = hBuffer[i];

		SetConsoleTitleA("Daily Planner");
		setConsoleSize(buffer, 69, 40);
		clearBuffer(buffer);
		SetConsoleCursorPosition(buffer, { 0, 0 });

		drawTitle(buffer);

		// 메뉴 출력
		writeBuffer(buffer, 24, 18, "Today is " + dateToStr(ymd));

		writeBuffer(buffer, 19, 20, "■〓〓〓〓〓〓〓〓〓〓〓〓〓■");
		writeBuffer(buffer, 26, 22, "Load by date");
		writeBuffer(buffer, 26, 22 + 2, "Load by category");
		writeBuffer(buffer, 26, 22 + 4, "Load after sorting by importance");
		writeBuffer(buffer, 26, 22 + 6, "Back to home screen");
		writeBuffer(buffer, 19, 22 + 8, "■〓〓〓〓〓〓〓〓〓〓〓〓〓■");
	}

	// 키보드 입력이 있을 때마다 2개의 버퍼가 계속해서 바뀌게 되어 깜빡거림 현상 해결
	while (true) {
		HANDLE buffer = hBuffer[!active];

		SetConsoleActiveScreenBuffer(buffer);	// 현재 출력되는 화면을 비활성화된 버퍼로 변경

		for (int y = 22; y <= 28; y += 2) {
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
				else if (keyInput == DOWN && posY < 28) {
					posY += 2;
				}
			}
			else if (keyInput == ENTER) {
				SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
				if (posY == 22) {
					showByDate();
					break;
				}
				else if (posY == 24) {
					showByCategory();
					break;
				}
				else if (posY == 26) {
					showByImportance();
					break;
				}
				else if (posY == 28) {
					mode = 0;
					break;
				}
			}
		}

		swapBuffer();
		Sleep(16);
	}
}

void LoadToDoScreen::showByDate()
{
	string dateInput;

	while (true) {
		cout << "Enter date[YYYY-MM-DD]: ";
		cin.ignore();
		getline(cin, dateInput);

		if (!isValidDate(dateInput)) {
			cout << "Invalid date. Try again." << endl;
		}
		else {
			break;
		}
	}
}

void LoadToDoScreen::showByCategory()
{
	string categoryInput;

	while (true) {
		cout << "Enter category: ";
		cin.ignore();
		getline(cin, categoryInput);

		if (!isValidCategory(dateInput)) {
			cout << "Invalid date. Try again." << endl;
		}
		else {
			break;
		}
	}
}

void LoadToDoScreen::showByImportance()
{

}
