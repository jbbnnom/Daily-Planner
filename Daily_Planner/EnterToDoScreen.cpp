#include "EnterToDoScreen.h"

using namespace std;

/* To-do 입력 창 */
void EnterToDoScreen::drawScreen(int& mode)
{
	int yearInput, monthInput, dayInput;
	int keyInput;


	// 사용자로부터 년도와 월 입력 후 설정
	cout << "----------Enter the information about date----------" << endl;
	while (true) {
		cout << "Enter year: ";
		cin >> yearInput;

		if (cin.fail()) {
			cin.clear();
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
			cerr << "Invalid month. Try again." << endl;
		}
		else {
			break;
		}
	}
	myPlanner.setYearMonth(yearInput, monthInput);

	// 현재 콘솔 화면 지우고 달력 출력
	system("cls");
	myPlanner.printCalendar();

	// 일 입력 후 to-do 입력
	moveCursor(10, 10);

	while (true) {
		cout << "Enter day: ";
		cin >> dayInput;

		if (cin.fail() || dayInput < 1 || dayInput > 12) {
			cin.clear();
			cerr << "Invalid month. Try again." << endl;
		}
		else {
			break;
		}
	}
	myPlanner.setDay(dayInput);

	system("cls");
	myPlanner.printCalendar();

	myPlanner.setYearMonthDay();
	chrono::weekday wd{ chrono::sys_days(myPlanner.getYearMonthDay()) };
	
	moveCursor(10, 10);
	cout << "You have selected " << dateToStr(myPlanner.getYearMonthDay())
		 << " [" << wd << "]\n" << endl;

	makeNewToDo();
	myPlanner.writeToFile(*todo);

	cout << "Your new to-do is successfully made! Press ESC to go home screen...";
	keyInput = _getch();
	if (_kbhit && keyInput == ESC) {
		mode = 0;
	}
	
	delete todo;
}

void EnterToDoScreen::makeNewToDo()
{
	string task, date, startTime, endTime, category;
	int importance;

	cout << "----------Enter the information about your To-do----------" << endl;

	cout << "Enter task: ";
	cin.ignore();
	getline(cin, task);

	while (true) {
		cout << "Enter date[YYYY-MM-DD]: ";
		cin.ignore();
		getline(cin, date);

		if (!isValidDate(date)) {
			cout << "Invalid date format. Try again." << endl;
		}
		else {
			break;
		}
	}

	while (true) {
		cout << "Enter start time [HH:MM:SS]: ";
		cin.ignore();
		getline(cin, startTime);

		if (!isValidTime(startTime)) {
			cout << "Invalid time format. Try again." << endl;
		}
		else {
			break;
		}
	}

	while (true) {
		cout << "Enter end time [HH:MM:SS]: ";
		cin.ignore();
		getline(cin, endTime);

		if (!isValidTime(endTime)) {
			cout << "Invalid time format. Try again." << endl;
		}
		else {
			break;
		}
	}

	cout << "Enter category: ";
	cin.ignore();
	getline(cin, category);

	cout << "Enter importance [lower number, lower importance | equal or greater than 0]: ";
	cin >> importance;
	
	todo = new ToDo(task, date, startTime, endTime, category, importance);	// check는 default parameter라 항상 false
}
