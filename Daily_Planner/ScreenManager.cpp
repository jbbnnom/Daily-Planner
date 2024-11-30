#include "ScreenManager.h"

using namespace std;

ScreenManager::ScreenManager()
{
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


// 매개변수로 주어진 좌표로 커서 이동_Default Console
void ScreenManager::moveCursor(SHORT x, SHORT y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 매개변수로 주어진 좌표로 커서 이동_User-made Buffer
void ScreenManager::moveCursor(HANDLE buffer, SHORT x, SHORT y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(buffer, pos);
}


// 주어진 버퍼에 내용을 작성(더블 버퍼링 구현 시 cout 사용이 제한되어 제작)
void ScreenManager::writeBuffer(HANDLE buffer, SHORT x, SHORT y, const char* content)
{
	DWORD charsWritten;

	moveCursor(buffer, x, y);
	WriteFile(buffer, content, strlen(content), &charsWritten, NULL);
}

// string 타입을 작성하기 위해 오버로드
void ScreenManager::writeBuffer(HANDLE buffer, SHORT x, SHORT y, const string& content)
{
	DWORD charsWritten;

	moveCursor(buffer, x, y);
	WriteFile(buffer, content.c_str(), content.size(), &charsWritten, NULL);
}


// 버퍼 크기만큼 공백을 입력해 현재 화면에 보이는 내용을 모두 제거
void ScreenManager::clearBuffer(HANDLE buffer)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(buffer, &csbi);
	COORD bufferSize = csbi.dwSize;

	DWORD charsWritten;
	FillConsoleOutputCharacterA(buffer, ' ', bufferSize.X * bufferSize.Y, { 0, 0 }, &charsWritten);
}


// 두 버퍼를 교환해 flickering 현상 방지
void ScreenManager::swapBuffer()
{
	SetConsoleActiveScreenBuffer(hBuffer[active]);
	active = !active;
}


// 매개변수로 받은 콘솔 창 및 버퍼의 크기를 조절 (width * height)
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