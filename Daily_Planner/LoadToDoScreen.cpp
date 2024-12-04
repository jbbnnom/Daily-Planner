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

	int keyInput, posY = 22, code;
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
		writeBuffer(buffer, 21, 18, "Today is " + dateToStr(ymd));

		writeBuffer(buffer, 13, 20, "■〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓■");
		writeBuffer(buffer, 20, 22, "Load by date");
		writeBuffer(buffer, 20, 22 + 2, "Load by category");
		writeBuffer(buffer, 20, 22 + 4, "Load after sorting by importance");
		writeBuffer(buffer, 20, 22 + 6, "Back to home screen");
		writeBuffer(buffer, 13, 22 + 8, "■〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓■");
	}

	// 키보드 입력이 있을 때마다 2개의 버퍼가 계속해서 바뀌게 되어 깜빡거림 현상 해결
	while (true) {
		HANDLE buffer = hBuffer[!active];

		SetConsoleActiveScreenBuffer(buffer);	// 현재 출력되는 화면을 비활성화된 버퍼로 변경

		for (int y = 22; y <= 28; y += 2) {
			writeBuffer(buffer, 16, y, "  ");
		}
		writeBuffer(buffer, 16, posY, "▶");

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
				if (posY == 22) {	// "Load by date"
					if (code = showByDate()) {
						mode = backToHomeScreen();
						return;
					}
					break;
				}
				else if (posY == 24) {	// "Load by category"
					if (code = showByCategory()) {
						mode = backToHomeScreen();
						return;
					}
					break;
				}
				else if (posY == 26) {	// "Load after sorting by importance"
					showByImportance();
					break;
				}
				else if (posY == 28) {	// "Back to home screen"
					mode = 0;
					return;
				}
			}
		}
		swapBuffer();
		Sleep(16);
	}

	// 사용자에게 할일 완료 여부 확인
	while (true) {
		cout << "Do you want to edit a to-do whether it is completed or not? (Y/N): ";
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
	mode = backToHomeScreen();
}

/* 사용자가 "Load by date" 선택 시 나오는 화면 출력 */
int LoadToDoScreen::showByDate()
{
	int code;
	string dateInput;

	system("mode con: cols=80 lines=40");

	moveCursor(0, 1);
	cout << "[Load by date]\n" << endl;

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
	myPlanner.printCalendar(1);


	// to-do list가 있는 경우에만 출력
	if (code = myTdm.getToDosByDate(myPlanner.getPlannerPath())) {
		cout << dateToStr(myPlanner.getYearMonthDay()) << "!" << endl;
		return 1;
	}
	else {
		moveCursor(24, 10);
		cout << "[To-do list in " << dateToStr(myPlanner.getYearMonthDay()) << "]\n" << endl;
		myTdm.printToDos_date();
	}

	return 0;
}

/* 사용자가 "Load by category" 선택 시 나오는 화면 출력 */
int LoadToDoScreen::showByCategory()
{
	string categoryInput;

	system("mode con: cols=82 lines=40");

	moveCursor(0, 1);
	cout << "[Load by category]\n" << endl;

	cout << "Enter category: ";
	if (cin.peek() == '\n') {
		cin.ignore();
	}
	getline(cin, categoryInput);

	myPlanner.loadAllToDos(myTdm);

	myTdm.getToDoByCategory(categoryInput);

	if (myTdm.getResult().empty()) {
		cout << "\nThe category you entered does not exist!" << endl;
		return 1;
	}
	else {
		system("cls");
		moveCursor(30, 1);
		cout << "[All to-dos in \'" << categoryInput << "\']\n" << endl;
		myTdm.printToDos_category();
	}
	
	return 0;
}

/* 사용자가 "Load after sorting by importance" 선택 시 나오는 화면 출력 */
void LoadToDoScreen::showByImportance()
{
	system("mode con: cols=92 lines=40");

	moveCursor(20, 1);
	cout << "[Load after sorting by importance]" << endl;

	myPlanner.loadAllToDos(myTdm);

	myTdm.sortToDoByImportance();

	system("cls");
	moveCursor(35, 1);
	cout << "[Sorted by Importance]\n" << endl;
	myTdm.printToDos_importance();
}

/* 사용자가 완료/미완료 여부를 변경 */
void LoadToDoScreen::editToDo()
{
	// 화면을 한번 없애고 시작
	system("cls");
	system("mode con: cols=97 lines=40");

	moveCursor(40, 1);
	cout << "[Edit mode]\n" << endl;

	// 사용자가 완료 여부를 수정하고자 하는 to-do를 받아와 저장 후 해당 날짜 추출
	ToDo target = myTdm.printToDos_editMode();
	string targetDate = target.getDate();

	// to-do의 날짜와 디렉터리 경로 설정, 반드시 파일 존재
	myPlanner.setYearMonthDay(stoi(targetDate.substr(0, 4)), stoi(targetDate.substr(5, 2)), stoi(targetDate.substr(8, 2)));
	myPlanner.setPlannerPath(0);	// year_month_day 객체로부터 설정하므로 0번 모드

	// 해당 날짜에 저장된 모든 to-do를 불러와 완료 여부 변경 후 덮어 씌우는 식으로 구현
	myTdm.getToDosByDate(myPlanner.getPlannerPath());
	myTdm.changeComplete(target);

	myPlanner.overlapToDos(myTdm);

	// 변경 성공 메시지 출력
	string compStr = (target.getCheck() == "Y") ? "Incomplete" : "Complete";
	cout << "\nYour \'" << target.getTask() << "\' task in " << dateToStr(myPlanner.getYearMonthDay())
		 << " has been successfully changed to \'" << compStr << "\'!" << endl;
}