#include "ScreenManager.h"

using namespace std;

/* Constructor defintion */
ScreenManager::ScreenManager(Planner& planner, ToDoManagement& tdm)
{
	myPlanner = planner;
	myTdm = tdm;

	// ���� ���۸��� ���� HANDLE Ÿ�� ���� 2���� ����
	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	active = false;

	// Ŀ���� �������� ���ְ� ũ�⸦ �ּҰ����� ���������ν� Ŀ���� ������� ��
	cursor.bVisible = false;
	cursor.dwSize = 1;

	// Ŀ�� ������ �� ���ۿ� �ݿ�
	SetConsoleCursorInfo(hBuffer[0], &cursor);
	SetConsoleCursorInfo(hBuffer[1], &cursor);
}


/* �Ű������� �־��� ��ǥ�� Ŀ�� �̵� - Default Console */
void ScreenManager::moveCursor(SHORT x, SHORT y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/* �Ű������� �־��� ��ǥ�� Ŀ�� �̵� - User-made Buffer */
void ScreenManager::moveCursor(HANDLE buffer, SHORT x, SHORT y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(buffer, pos);
}


/* �־��� ���ۿ� ������ �ۼ� - C-style ���ڿ�
 * (���� ���۸� ���� �� cout ����� ���ѵǾ� ����) */
void ScreenManager::writeBuffer(HANDLE buffer, SHORT x, SHORT y, const char* content)
{
	DWORD charsWritten;

	moveCursor(buffer, x, y);
	WriteFile(buffer, content, strlen(content), &charsWritten, NULL);
}

/* �־��� ���ۿ� ������ �ۼ� - string Ÿ�� ���ڿ� */
void ScreenManager::writeBuffer(HANDLE buffer, SHORT x, SHORT y, const string& content)
{
	DWORD charsWritten;

	moveCursor(buffer, x, y);
	WriteFile(buffer, content.c_str(), content.size(), &charsWritten, NULL);
}


/* ���� ũ�⸸ŭ ������ �Է��� ���� ȭ�鿡 ���̴� ������ ��� ���� */
void ScreenManager::clearBuffer(HANDLE buffer)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(buffer, &csbi);
	COORD bufferSize = csbi.dwSize;

	DWORD charsWritten;
	FillConsoleOutputCharacterA(buffer, ' ', bufferSize.X * bufferSize.Y, { 0, 0 }, &charsWritten);
}


/* �� ���۸� ��ȯ�� flickering ���� ���� */
void ScreenManager::swapBuffer()
{
	SetConsoleActiveScreenBuffer(hBuffer[active]);
	active = !active;
}


/* �Ű������� ���� �ܼ� â �� ������ ũ�⸦ ���� (width * height) */
void ScreenManager::setConsoleSize(HANDLE buffer, SHORT width, SHORT height)
{
    // #1: ������ ��ȯ�� ���� ���� â ũ�⸦ �ٿ��� ��
	// (�� ������ ��ġ�� ������ â ũ�Ⱑ default���� ������ ����)
    SMALL_RECT smallScreen = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(buffer, TRUE, &smallScreen);

    // #2. ���� ũ����� ����
    COORD bufferSize = { width, height };
	SetConsoleScreenBufferSize(buffer, bufferSize);

    // #3. â ũ�� ����
    SMALL_RECT windowSize = { 0, 0, width - 1, height - 1 };	// SMALL_RECT ����ü�� Ư��(inclusive coordinate) ������ width�� height���� 1�� ����� ��
	SetConsoleWindowInfo(buffer, TRUE, &windowSize);

	// #4. ���α׷� ���� �� â ũ�� ���� ���� (���� ���� ���� �� UI ����)
	HWND hwndConsole = GetConsoleWindow();
	LONG style = GetWindowLong(hwndConsole, GWL_STYLE);
	style &= ~WS_SIZEBOX;         // â �׵θ� ���콺 Ŭ�� ���� ��Ȱ��ȭ
	style &= ~WS_MAXIMIZEBOX;     // �ִ�ȭ ��ư ��Ȱ��ȭ
	style &= ~WS_MINIMIZEBOX;     // �ּ�ȭ ��ư ��Ȱ��ȭ
	SetWindowLong(hwndConsole, GWL_STYLE, style);
	SetWindowPos(hwndConsole, NULL, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

/* Ÿ��Ʋ ��� */
void ScreenManager::drawTitle(HANDLE handle)
{
	string s;

	s += "\n  ########     ###    #### ##       ##    ##\n";
	s += "  ##     ##   ## ##    ##  ##        ##  ## \n";
	s += "  ##     ##  ##   ##   ##  ##         ####  \n";
	s += "  ##     ## ##     ##  ##  ##          ##   \n";
	s += "  ##     ## #########  ##  ##          ##   \n";
	s += "  ##     ## ##     ##  ##  ##          ##   \n";
	s += "  ########  ##     ## #### ########    ##   \n\n";

	s += "  ########  ##          ###    ##    ## ##    ## ######## ######## \n";
	s += "  ##     ## ##         ## ##   ###   ## ###   ## ##       ##     ##\n";
	s += "  ##     ## ##        ##   ##  ####  ## ####  ## ##       ##     ##\n";
	s += "  ########  ##       ##     ## ## ## ## ## ## ## ######   ######## \n";
	s += "  ##        ##       ######### ##  #### ##  #### ##       ##   ##  \n";
	s += "  ##        ##       ##     ## ##   ### ##   ### ##       ##    ## \n";
	s += "  ##        ######## ##     ## ##    ## ##    ## ######## ##     ##\n\n\n";

	writeBuffer(handle, 0, 0, s);
}

bool ScreenManager::isValidDate(const string& date)
{
	// Regular expression to match the format YYYY-MM-DD
	regex dateFormat(R"(\d{4}-\d{2}-\d{2})");

	if (regex_match(date, dateFormat)) {
		// Further validation for realistic dates (e.g., month <= 12, day <= 31)
		int year, month, day;
		if (sscanf(date.c_str(), "%4d-%2d-%2d", &year, &month, &day) == 3) {
			if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
				return true;
			}
		}
	}
	return false;
}

bool ScreenManager::isValidTime(const string& time)
{
	// Regular expression to match the format YYYY-MM-DD
	regex timeFormat(R"(\d{2}-\d{2}-\d{2})");

	if (regex_match(time, timeFormat)) {
		// Further validation for realistic dates (e.g., month <= 12, day <= 31)
		int hour, minute, second;
		if (sscanf(time.c_str(), "%2d-%2d-%2d", &hour, &minute, &second) == 3) {
			if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59 && second >= 0 && second <= 59) {
				return true;
			}
		}
	}
	return false;
}

bool ScreenManager::isValidCategory(const string& category)
{
	return false;
}
