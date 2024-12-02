#include "LoadToDoScreen.h"

using namespace std;

/* To-Do �ҷ����� â 
 * Ȩ ȭ��� ���������� ���� ���۸� ����� �����
 */
void LoadToDoScreen::drawScreen(int& mode)
{
	// ���ۿ� �������� �� �ִ� Ű �Է� ����
	while (_kbhit()) {
		_getch();
	}

	system("mode con: cols=69 lines=40");	// �⺻ â ũ�� ����

	int keyInput, posY = 22;
	string userInput;

	// ���� ��¥�� �޾ƿ� ����
	auto now = chrono::system_clock::now();
	auto now_time = chrono::floor<chrono::days>(now);
	auto ymd = chrono::year_month_day{ now_time };

	// ���� ���۸��� ���� �غ��� 2���� ���� ���� ����
	for (int i = 0; i < 2; i++) {
		HANDLE buffer = hBuffer[i];

		SetConsoleTitleA("Daily Planner");
		setConsoleSize(buffer, 69, 40);
		clearBuffer(buffer);
		SetConsoleCursorPosition(buffer, { 0, 0 });

		drawTitle(buffer);

		// �޴� ���
		writeBuffer(buffer, 24, 18, "Today is " + dateToStr(ymd));

		writeBuffer(buffer, 19, 20, "����������������");
		writeBuffer(buffer, 26, 22, "Load by date");
		writeBuffer(buffer, 26, 22 + 2, "Load by category");
		writeBuffer(buffer, 26, 22 + 4, "Load after sorting by importance");
		writeBuffer(buffer, 26, 22 + 6, "Back to home screen");
		writeBuffer(buffer, 19, 22 + 8, "����������������");
	}

	// Ű���� �Է��� ���� ������ 2���� ���۰� ����ؼ� �ٲ�� �Ǿ� �����Ÿ� ���� �ذ�
	while (true) {
		HANDLE buffer = hBuffer[!active];

		SetConsoleActiveScreenBuffer(buffer);	// ���� ��µǴ� ȭ���� ��Ȱ��ȭ�� ���۷� ����

		for (int y = 22; y <= 28; y += 2) {
			writeBuffer(buffer, 22, y, "  ");
		}
		writeBuffer(buffer, 22, posY, "��");

		// Ű���� �Է�
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
			else if (keyInput == ENTER) {	// ���Ͱ� �ԷµǸ� ǥ�� ȭ�� ������� ���� ����
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

	// ����ڿ��� ���� ���� Ȯ�� - ���� �̱���
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

/* ����ڰ� "Load by date" ���� �� ������ ȭ�� ��� */
void LoadToDoScreen::showByDate()
{
	int code;
	string dateInput;

	system("mode con: cols=80 lines=40");

	while (true) {
		cout << "Enter date[YYYY-MM-DD]: ";
		if (cin.peek() == '\n') {	// proj1���� �߻��� �������� ������ �ݺ��Ǿ� �߻��� �ذ�
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

	// ��¥ �� ���� ��� ���� �� �� ���� to-do �ҷ�����
	myPlanner.setYearMonth(stoi(dateInput.substr(0, 4)), stoi(dateInput.substr(5, 2)));
	myPlanner.setYearMonthDay(stoi(dateInput.substr(0, 4)), stoi(dateInput.substr(5, 2)), stoi(dateInput.substr(8, 2)));
	myPlanner.setPlannerPath(0);

	system("cls");
	myPlanner.printCalendar();


	// to-do list�� �ִ� ��쿡�� ���
	if (code = myTdm.getToDosByDate(myPlanner.getPlannerPath())) {
		cout << dateToStr(myPlanner.getYearMonthDay()) << "!" << endl;
	}
	else {
		moveCursor(10, 10);
		cout << "To-do list in " << dateToStr(myPlanner.getYearMonthDay()) << endl;
		myTdm.printToDos_date();
	}
}

/* ����ڰ� "Load by category" ���� �� ������ ȭ�� ��� */
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

/* ����ڰ� "Load after sorting by importance" ���� �� ������ ȭ�� ��� */
void LoadToDoScreen::showByImportance()
{
	myPlanner.loadAllToDos(myTdm);

	myTdm.sortToDoByImportance();

	system("cls");
	cout << "Sorted by Importance" << endl;
	myTdm.printToDos_importance();
}

/* to-do�� ��ü ������ �����ؾ� �ϴ°�, �ƴϸ� üũ ���θ� ������ �����ؾ� �ϴ°�.. */
void LoadToDoScreen::editToDo()
{
	ToDo target = myTdm.printToDos_editMode();

	// ���� ��� ���� �ʿ�..
}
