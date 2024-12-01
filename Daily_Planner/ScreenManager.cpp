#include "ScreenManager.h"

using namespace std;

/* Constructor defintion */
ScreenManager::ScreenManager(Planner& planner, ToDoManagement& tdm)
{
	myPlanner = planner;
	myTdm = tdm;

	// 더블 버퍼링을 위해 HANDLE 타입 버퍼 2개를 생성
	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	active = false;

	// 커서의 깜박임을 없애고 크기를 최소값으로 지정함으로써 커서가 사라지게 됨
	cursor.bVisible = false;
	cursor.dwSize = 1;

	// 커서 설정을 각 버퍼에 반영
	SetConsoleCursorInfo(hBuffer[0], &cursor);
	SetConsoleCursorInfo(hBuffer[1], &cursor);
}


/* 매개변수로 주어진 좌표로 커서 이동 - Default Console */
void ScreenManager::moveCursor(SHORT x, SHORT y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/* 매개변수로 주어진 좌표로 커서 이동 - User-made Buffer */
void ScreenManager::moveCursor(HANDLE buffer, SHORT x, SHORT y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(buffer, pos);
}


/* 주어진 버퍼에 내용을 작성 - C-style 문자열
 * (더블 버퍼링 구현 시 cout 사용이 제한되어 제작) */
void ScreenManager::writeBuffer(HANDLE buffer, SHORT x, SHORT y, const char* content)
{
	DWORD charsWritten;

	moveCursor(buffer, x, y);
	WriteFile(buffer, content, strlen(content), &charsWritten, NULL);
}

/* 주어진 버퍼에 내용을 작성 - string 타입 문자열 */
void ScreenManager::writeBuffer(HANDLE buffer, SHORT x, SHORT y, const string& content)
{
	DWORD charsWritten;

	moveCursor(buffer, x, y);
	WriteFile(buffer, content.c_str(), content.size(), &charsWritten, NULL);
}


/* 버퍼 크기만큼 공백을 입력해 현재 화면에 보이는 내용을 모두 제거 */
void ScreenManager::clearBuffer(HANDLE buffer)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(buffer, &csbi);
	COORD bufferSize = csbi.dwSize;

	DWORD charsWritten;
	FillConsoleOutputCharacterA(buffer, ' ', bufferSize.X * bufferSize.Y, { 0, 0 }, &charsWritten);
}


/* 두 버퍼를 교환해 flickering 현상 방지 */
void ScreenManager::swapBuffer()
{
	SetConsoleActiveScreenBuffer(hBuffer[active]);
	active = !active;
}


/* 매개변수로 받은 콘솔 창 및 버퍼의 크기를 조절 (width * height) */
void ScreenManager::setConsoleSize(HANDLE buffer, SHORT width, SHORT height)
{
    // #1: 원할한 변환을 위해 현재 창 크기를 줄여야 함
	// (이 과정을 거치지 않으면 창 크기가 default에서 변하지 않음)
    SMALL_RECT smallScreen = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(buffer, TRUE, &smallScreen);

    // #2. 버퍼 크기부터 조절
    COORD bufferSize = { width, height };
	SetConsoleScreenBufferSize(buffer, bufferSize);

    // #3. 창 크기 조절
    SMALL_RECT windowSize = { 0, 0, width - 1, height - 1 };	// SMALL_RECT 구조체의 특성(inclusive coordinate) 때문에 width와 height에서 1을 빼줘야 함
	SetConsoleWindowInfo(buffer, TRUE, &windowSize);

	// #4. 프로그램 실행 시 창 크기 변경 제한 (글자 깨짐 방지 및 UI 유지)
	HWND hwndConsole = GetConsoleWindow();
	LONG style = GetWindowLong(hwndConsole, GWL_STYLE);
	style &= ~WS_SIZEBOX;         // 창 테두리 마우스 클릭 조절 비활성화
	style &= ~WS_MAXIMIZEBOX;     // 최대화 버튼 비활성화
	style &= ~WS_MINIMIZEBOX;     // 최소화 버튼 비활성화
	SetWindowLong(hwndConsole, GWL_STYLE, style);
	SetWindowPos(hwndConsole, NULL, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

/* 타이틀 출력 */
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
