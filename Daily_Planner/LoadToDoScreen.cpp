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

	int keyInput, posY = 22, code;
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
		writeBuffer(buffer, 21, 18, "Today is " + dateToStr(ymd));

		writeBuffer(buffer, 13, 20, "����������������������");
		writeBuffer(buffer, 20, 22, "Load by date");
		writeBuffer(buffer, 20, 22 + 2, "Load by category");
		writeBuffer(buffer, 20, 22 + 4, "Load after sorting by importance");
		writeBuffer(buffer, 20, 22 + 6, "Back to home screen");
		writeBuffer(buffer, 13, 22 + 8, "����������������������");
	}

	// Ű���� �Է��� ���� ������ 2���� ���۰� ����ؼ� �ٲ�� �Ǿ� �����Ÿ� ���� �ذ�
	while (true) {
		HANDLE buffer = hBuffer[!active];

		SetConsoleActiveScreenBuffer(buffer);	// ���� ��µǴ� ȭ���� ��Ȱ��ȭ�� ���۷� ����

		for (int y = 22; y <= 28; y += 2) {
			writeBuffer(buffer, 16, y, "  ");
		}
		writeBuffer(buffer, 16, posY, "��");

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

	// ����ڿ��� ���� �Ϸ� ���� Ȯ��
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

/* ����ڰ� "Load by date" ���� �� ������ ȭ�� ��� */
int LoadToDoScreen::showByDate()
{
	int code;
	string dateInput;

	system("mode con: cols=80 lines=40");

	moveCursor(0, 1);
	cout << "[Load by date]\n" << endl;

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
	myPlanner.printCalendar(1);


	// to-do list�� �ִ� ��쿡�� ���
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

/* ����ڰ� "Load by category" ���� �� ������ ȭ�� ��� */
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

/* ����ڰ� "Load after sorting by importance" ���� �� ������ ȭ�� ��� */
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

/* ����ڰ� �Ϸ�/�̿Ϸ� ���θ� ���� */
void LoadToDoScreen::editToDo()
{
	// ȭ���� �ѹ� ���ְ� ����
	system("cls");
	system("mode con: cols=97 lines=40");

	moveCursor(40, 1);
	cout << "[Edit mode]\n" << endl;

	// ����ڰ� �Ϸ� ���θ� �����ϰ��� �ϴ� to-do�� �޾ƿ� ���� �� �ش� ��¥ ����
	ToDo target = myTdm.printToDos_editMode();
	string targetDate = target.getDate();

	// to-do�� ��¥�� ���͸� ��� ����, �ݵ�� ���� ����
	myPlanner.setYearMonthDay(stoi(targetDate.substr(0, 4)), stoi(targetDate.substr(5, 2)), stoi(targetDate.substr(8, 2)));
	myPlanner.setPlannerPath(0);	// year_month_day ��ü�κ��� �����ϹǷ� 0�� ���

	// �ش� ��¥�� ����� ��� to-do�� �ҷ��� �Ϸ� ���� ���� �� ���� ����� ������ ����
	myTdm.getToDosByDate(myPlanner.getPlannerPath());
	myTdm.changeComplete(target);

	myPlanner.overlapToDos(myTdm);

	// ���� ���� �޽��� ���
	string compStr = (target.getCheck() == "Y") ? "Incomplete" : "Complete";
	cout << "\nYour \'" << target.getTask() << "\' task in " << dateToStr(myPlanner.getYearMonthDay())
		 << " has been successfully changed to \'" << compStr << "\'!" << endl;
}