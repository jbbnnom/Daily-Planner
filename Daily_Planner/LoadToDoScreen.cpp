#include "LoadToDoScreen.h"

using namespace std;

/* To-Do 불러오기 창 
 * 홈 화면과 마찬가지로 더블 버퍼링 기법이 적용됨
 */
void LoadToDoScreen::drawScreen(int& mode)
{
	// 버퍼에 남아있을 수 있는 키 입력 제거
	while (_kbhit()) {
		_getch();
	}

	system("mode con: cols=69 lines=40");	// 기본 창 크기 설정

	int keyInput, posY = 22;
	string userInput;

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
			else if (keyInput == ENTER) {	// 엔터가 입력되면 표준 화면 출력으로 버퍼 변경
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
					return;
				}
			}
		}

		swapBuffer();
		Sleep(16);
	}

	// 사용자에게 편집 여부 확인 - 아직 미구현
	while (true) {
		cout << "Do you want to edit a to-do? (Y/N): ";
		cin >> userInput;

		if (userInput != "Y" && userInput != "y" && userInput != "N" && userInput != "n") {
			cout << "Invalid input. Try again." << endl;
		}
		else if (userInput == "N" || userInput == "n") {
			mode = backToHomeScreen();
			return;
		}
		else {
			editToDo();
			break;
		}
	}
}

/* 사용자가 "Load by date" 선택 시 나오는 화면 출력 */
void LoadToDoScreen::showByDate()
{
	int code;
	string dateInput;

	system("mode con: cols=80 lines=40");

	while (true) {
		cout << "Enter date[YYYY-MM-DD]: ";
		if (cin.peek() == '\n') {	// proj1에서 발생한 고질적인 문제가 반복되어 발생해 해결
			cin.ignore();
		}
		getline(cin, dateInput);

		if (!myTdm.isValidDate(dateInput)) {
			cout << "Invalid date format. Try again." << endl;
		}
		else {
			break;
		}
	}

	// 날짜 및 파일 경로 설정 후 그 날의 to-do 불러오기
	myPlanner.setYearMonth(stoi(dateInput.substr(0, 4)), stoi(dateInput.substr(5, 2)));
	myPlanner.setYearMonthDay(stoi(dateInput.substr(0, 4)), stoi(dateInput.substr(5, 2)), stoi(dateInput.substr(8, 2)));
	myPlanner.setPlannerPath(0);

	system("cls");
	myPlanner.printCalendar();


	// to-do list가 있는 경우에만 출력
	if (code = myTdm.getToDosByDate(myPlanner.getPlannerPath())) {
		cout << dateToStr(myPlanner.getYearMonthDay()) << "!" << endl;
	}
	else {
		moveCursor(10, 10);
		cout << "To-do list in " << dateToStr(myPlanner.getYearMonthDay()) << endl;
		myTdm.printToDos_date();
	}
}

/* 사용자가 "Load by category" 선택 시 나오는 화면 출력 */
void LoadToDoScreen::showByCategory()
{
	string categoryInput;

	cout << "Enter category: ";
	if (cin.peek() == '\n') {
		cin.ignore();
	}
	getline(cin, categoryInput);
	cout << categoryInput << endl;

	myPlanner.loadAllToDos(myTdm);

	myTdm.getToDoByCategory(categoryInput);

	if (myTdm.getResult().empty()) {
		cout << "The category you entered does not exist!" << endl;
	}
	else {
		system("cls");
		cout << "All to-dos in \'" << categoryInput << "\'" << endl;
		myTdm.printToDos_category();
	}
}

/* 사용자가 "Load after sorting by importance" 선택 시 나오는 화면 출력 */
void LoadToDoScreen::showByImportance()
{
	myPlanner.loadAllToDos(myTdm);

	myTdm.sortToDoByImportance();

	system("cls");
	cout << "Sorted by Importance" << endl;
	myTdm.printToDos_importance();
}

/* to-do의 전체 수정이 가능해야 하는가, 아니면 체크 여부만 수정이 가능해야 하는가.. */
void LoadToDoScreen::editToDo()
{
	ToDo target = myTdm.printToDos_editMode();

	// 수정 모드 구현 필요..
}
