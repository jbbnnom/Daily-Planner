#ifndef _SCREEN_MANAGER_H_
#define _SCREEN_MANAGER_H_

#include "Planner.h"

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <chrono>
#include <iomanip>
#include <string>

#define ARROW 224
#define UP    72
#define DOWN  80
#define ENTER 13
#define ESC   27


class ScreenManager {
protected:
	HANDLE hBuffer[2];
	bool active;
	CONSOLE_CURSOR_INFO cursor;

public:
	ScreenManager();
	virtual ~ScreenManager()
	{
		CloseHandle(hBuffer[0]);
		CloseHandle(hBuffer[1]);
	}

	virtual void drawScreen(int& mode) = 0;

	static void moveCursor(SHORT x, SHORT y);
	static void moveCursor(HANDLE buffer, SHORT x, SHORT y);
	void writeBuffer(HANDLE buffer, SHORT x, SHORT y, const char* content);
	void writeBuffer(HANDLE buffer, SHORT x, SHORT y, const std::string& content);
	void clearBuffer(HANDLE buffer);
	void swapBuffer();
	void setConsoleSize(HANDLE buffer, SHORT width, SHORT height);
};

#endif