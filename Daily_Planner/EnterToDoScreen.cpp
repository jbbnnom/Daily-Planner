#include "EnterToDoScreen.h"

using namespace std;

/* To-do 입력 창 */
void EnterToDoScreen::drawScreen(int& mode)
{
	int yearInput, monthInput, dayInput;

	// 사용자로부터 년도와 월 입력 후 설정
	cout << "\n------------------[Enter the information about date]-----------------" << endl;
	while (true) {
		cout << "Enter year: ";
		cin >> yearInput;

		if (cin.fail()) {	// 유효성 확인
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

	// 현재 콘솔 화면 지우고 달력 출력
	system("cls");
	myPlanner.printCalendar(0);

	// 일 입력
	moveCursor(0, 10);
	while (true) {
		cout << "Enter day: ";
		cin >> dayInput;

		if (cin.fail() || dayInput < 1 || dayInput > 31) {
			cin.clear();
			cin.ignore(32767, '\n');
			cerr << "Invalid month. Try again." << endl;
		}
		else {
			break;
		}
	}
	myPlanner.setDay(dayInput);

	
	// 사용자에게 입력한 날짜 보여주고 to-do 입력 모드 진입
	system("cls");
	myPlanner.printCalendar(0);

	myPlanner.setYearMonthDay();
	chrono::weekday wd{ chrono::sys_days(myPlanner.getYearMonthDay()) };
	
	moveCursor(13, 10);
	cout << "You have selected " << dateToStr(myPlanner.getYearMonthDay())
		 << " [" << wd << "]\n\n" << endl;

	// 입력한 to-do는 ToDoManagement 클래스의 newTodo 멤버로 동적 할당되어 저장
	myTdm.makeNewToDo();

	// 날짜는 별도로 설정 필요
	string date;
	date += to_string(yearInput);
	date += "-";
	if (monthInput < 10) {
		date += "0";
	}
	date += to_string(monthInput);
	date += "-";
	if (dayInput < 10) {
		date += "0";
	}
	date += to_string(dayInput);

	ToDo ntd = myTdm.getNewToDo();
	ntd.changeDate(date);
	myPlanner.writeToFile(ntd);

	// newTodo 멤버 메모리 할당 해제 후 홈 화면으로 복귀
	myTdm.deallocToDo();

	cout << "\nNew to-do is successfully made!" << endl;
	mode = backToHomeScreen();
}
