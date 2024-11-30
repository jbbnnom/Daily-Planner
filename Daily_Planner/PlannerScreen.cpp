#include "PlannerScreen.h"

using namespace std;

// �÷��� â
void PlannerScreen::drawScreen(int& mode)
{
	int yearInput, monthInput, dayInput;
	int k;

	// ����ڷκ��� �⵵�� �� �Է� �� ����
	cout << "Enter year: ";
	cin >> yearInput;
	cout << "Enter month: ";
	cin >> monthInput;

	if (monthInput < 1 || monthInput > 12) {
		cerr << "Invalid month. Try again." << endl;
		return;
	}

	planner.setYearMonth(yearInput, monthInput);

	// ���� �ܼ� ȭ�� ����� �޷� ���
	system("cls");
	planner.printCalendar();

	moveCursor(10, 10);
	cout << "Enter day: ";
	cin >> dayInput;
	planner.setDay(dayInput);

	planner.writeToFile();

	/*k = _getch();*/
	
}
