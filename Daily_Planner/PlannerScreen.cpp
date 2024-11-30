#include "PlannerScreen.h"

using namespace std;

// 플래너 창
void PlannerScreen::drawScreen(int& mode)
{
	int yearInput, monthInput, dayInput;
	int k;

	// 사용자로부터 년도와 월 입력 후 설정
	cout << "Enter year: ";
	cin >> yearInput;
	cout << "Enter month: ";
	cin >> monthInput;

	if (monthInput < 1 || monthInput > 12) {
		cerr << "Invalid month. Try again." << endl;
		return;
	}

	planner.setYearMonth(yearInput, monthInput);

	// 현재 콘솔 화면 지우고 달력 출력
	system("cls");
	planner.printCalendar();

	moveCursor(10, 10);
	cout << "Enter day: ";
	cin >> dayInput;
	planner.setDay(dayInput);

	planner.writeToFile();

	/*k = _getch();*/
	
}
