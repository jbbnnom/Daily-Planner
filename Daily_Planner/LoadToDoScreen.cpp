#include "LoadToDoScreen.h"

using namespace std;

/* To-Do �ҷ����� â */
void LoadToDoScreen::drawScreen(int& mode)
{
	system("mode con: cols=69 lines=40");	// �⺻ â ũ�� ����

	int keyInput, posY = 22;

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
			else if (keyInput == ENTER) {
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
					break;
				}
			}
		}

		swapBuffer();
		Sleep(16);
	}
}

void LoadToDoScreen::showByDate()
{
	string dateInput;

	while (true) {
		cout << "Enter date[YYYY-MM-DD]: ";
		cin.ignore();
		getline(cin, dateInput);

		if (!isValidDate(dateInput)) {
			cout << "Invalid date. Try again." << endl;
		}
		else {
			break;
		}
	}
}

void LoadToDoScreen::showByCategory()
{
	string categoryInput;

	while (true) {
		cout << "Enter category: ";
		cin.ignore();
		getline(cin, categoryInput);

		if (!isValidCategory(dateInput)) {
			cout << "Invalid date. Try again." << endl;
		}
		else {
			break;
		}
	}
}

void LoadToDoScreen::showByImportance()
{

}
