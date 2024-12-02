#include "HomeScreen.h"

using namespace std;
namespace fs = std::filesystem;

/* [메인 화면 출력]
 * 하나의 버퍼가 출력될 때 나머지 하나의 버퍼에 그 내용을 복사하고
 * 이 둘을 계속해서 swap하는 방법으로 flickering 현상 해결
 * (system("cls")를 사용하는 방식으로 하나의 버퍼로만 운용하면 flickering 현상 발생)
 */
void HomeScreen::drawScreen(int& mode)
{
	// 버퍼에 남아있을 수 있는 키 입력 제거
	while (_kbhit()) {
		_getch();
	}

	system("mode con: cols=69 lines=40");	// 기본 창 크기 설정

	int keyInput, posY = 30;

	// 오늘 날짜를 받아와 저장
	auto now = chrono::system_clock::now();
	auto now_time = chrono::floor<chrono::days>(now);
	auto ymd = chrono::year_month_day{ now_time };

	myPlanner.setYearMonthDay(ymd);
	myPlanner.setPlannerPath(0);	// ymd 객체로 디렉터리 설정

	// 더블 버퍼링을 위해 준비한 2개의 버퍼 설정 적용
	for (int i = 0; i < 2; i++) {
		HANDLE buffer = hBuffer[i];

		SetConsoleTitleA("Daily Planner");
		setConsoleSize(buffer, 69, 40);
		clearBuffer(buffer);
		SetConsoleCursorPosition(buffer, { 0, 0 });

		drawTitle(buffer);
		
		// 오늘의 할 일 및 메뉴 출력
		writeBuffer(buffer, 24, 18, "Today is " + dateToStr(ymd));
		writeBuffer(buffer, 24, 20, "Today\'s to-do list");
		if (!fs::exists(myPlanner.getPlannerPath())) {
			writeBuffer(buffer, 24, 22, "There is nothing to do. Enjoy your day!\n");
		}
		else {
			// 있는 경우...
		}

		writeBuffer(buffer, 19, 28, "■〓〓〓〓〓〓〓〓〓〓〓〓〓■");
		writeBuffer(buffer, 26, 30, "Enter new To-do");
		writeBuffer(buffer, 26, 30 + 2, "Load and Check To-dos");
		writeBuffer(buffer, 26, 30 + 4, "Exit");
		writeBuffer(buffer, 19, 36, "■〓〓〓〓〓〓〓〓〓〓〓〓〓■");
	}

	// 키보드 입력이 있을 때마다 2개의 버퍼가 계속해서 바뀌게 되어 깜빡거림 현상 해결
	while (true) {
		HANDLE buffer = hBuffer[!active];

		SetConsoleActiveScreenBuffer(buffer);	// 현재 출력되는 화면을 비활성화된 버퍼로 변경

		// 메뉴 화살표 운용
		for (int y = 30; y <= 34; y += 2) {
			writeBuffer(buffer, 22, y, "  ");
		}
		writeBuffer(buffer, 22, posY, "▶");

		// 키보드 입력
		if (_kbhit()) {	// 키 입력이 확인되면...
			keyInput = _getch();	// 그 입력을 받아 keyInput에 저장
			if (keyInput == ARROW) {
				keyInput = _getch();	// 방향키는 입력되면 ARROW(224)와 방향에 따른 값(UP-72/DOWN-80)이 총 2번 입력도니다 
				if (keyInput == UP && posY > 30) {
					posY -= 2;
				}
				else if (keyInput == DOWN && posY < 36) {
					posY += 2;
				}
			}
			else if (keyInput == ENTER) {	// 엔터키 입력 시 해당 스크린으로 전환
				if (posY == 30) {	// Enter new To-do
					mode = 1;
					break;
				}
				else if (posY == 32) {	// Load and Check To-dos
					mode = 2;
					break;
				}
				else if (posY == 34) {	// Exit
					exit(0);
				}
			}
		}

		// 2개의 버퍼를 교환
		swapBuffer();
		Sleep(16);
	}
}