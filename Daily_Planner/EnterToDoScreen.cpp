#include "EnterToDoScreen.h"

using namespace std;

/* To-do �Է� â */
void EnterToDoScreen::drawScreen(int& mode)
{
	int yearInput, monthInput, dayInput;

	// ����ڷκ��� �⵵�� �� �Է� �� ����
	cout << "----------Enter the information about date----------" << endl;
	while (true) {
		cout << "Enter year: ";
		cin >> yearInput;

		if (cin.fail()) {	// ��ȿ�� Ȯ��
			cin.clear();
			cin.ignore(32767, '\n');
			cerr << "Invalid year. Try again." << endl;
		}
		else {
			break;
		}
	}
	while (true) {
		cout << "Enter month: ";
		cin >> monthInput;

		if (cin.fail() || monthInput < 1 || monthInput > 12) {
			cin.clear();
			cin.ignore(32767, '\n');
			cerr << "Invalid month. Try again." << endl;
		}
		else {
			break;
		}
	}
	myPlanner.setYearMonth(yearInput, monthInput);

	// ���� �ܼ� ȭ�� ����� �޷� ���
	system("cls");
	myPlanner.printCalendar();

	// �� �Է�
	moveCursor(10, 10);
	while (true) {
		cout << "Enter day: ";
		cin >> dayInput;

		if (cin.fail() || dayInput < 1 || dayInput > 12) {
			cin.clear();
			cin.ignore(32767, '\n');
			cerr << "Invalid month. Try again." << endl;
		}
		else {
			break;
		}
	}
	myPlanner.setDay(dayInput);


	// ����ڿ��� �Է��� ��¥ �����ְ� to-do �Է� ��� ����
	system("cls");
	myPlanner.printCalendar();

	myPlanner.setYearMonthDay();
	chrono::weekday wd{ chrono::sys_days(myPlanner.getYearMonthDay()) };
	
	moveCursor(10, 10);
	cout << "You have selected " << dateToStr(myPlanner.getYearMonthDay())
		 << " [" << wd << "]\n" << endl;

	// �Է��� to-do�� ToDoManagement Ŭ������ newTodo ����� ���� �Ҵ�Ǿ� ����
	myTdm.makeNewToDo();
	myPlanner.writeToFile(myTdm.getNewToDo());

	// newTodo ��� �޸� �Ҵ� ���� �� Ȩ ȭ������ ����
	myTdm.deallocToDo();

	cout << "New to-do is successfully made!" << endl;
	mode = backToHomeScreen();
}
