#include "ScreenManager.h"

using namespace std;

ScreenManager::ScreenManager()
{
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


// �Ű������� �־��� ��ǥ�� Ŀ�� �̵�_Default Console
void ScreenManager::moveCursor(SHORT x, SHORT y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// �Ű������� �־��� ��ǥ�� Ŀ�� �̵�_User-made Buffer
void ScreenManager::moveCursor(HANDLE buffer, SHORT x, SHORT y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(buffer, pos);
}


// �־��� ���ۿ� ������ �ۼ�(���� ���۸� ���� �� cout ����� ���ѵǾ� ����)
void ScreenManager::writeBuffer(HANDLE buffer, SHORT x, SHORT y, const char* content)
{
	DWORD charsWritten;

	moveCursor(buffer, x, y);
	WriteFile(buffer, content, strlen(content), &charsWritten, NULL);
}

// string Ÿ���� �ۼ��ϱ� ���� �����ε�
void ScreenManager::writeBuffer(HANDLE buffer, SHORT x, SHORT y, const string& content)
{
	DWORD charsWritten;

	moveCursor(buffer, x, y);
	WriteFile(buffer, content.c_str(), content.size(), &charsWritten, NULL);
}


// ���� ũ�⸸ŭ ������ �Է��� ���� ȭ�鿡 ���̴� ������ ��� ����
void ScreenManager::clearBuffer(HANDLE buffer)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(buffer, &csbi);
	COORD bufferSize = csbi.dwSize;

	DWORD charsWritten;
	FillConsoleOutputCharacterA(buffer, ' ', bufferSize.X * bufferSize.Y, { 0, 0 }, &charsWritten);
}


// �� ���۸� ��ȯ�� flickering ���� ����
void ScreenManager::swapBuffer()
{
	SetConsoleActiveScreenBuffer(hBuffer[active]);
	active = !active;
}


// �Ű������� ���� �ܼ� â �� ������ ũ�⸦ ���� (width * height)
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