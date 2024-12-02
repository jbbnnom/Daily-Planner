#ifndef _SCREEN_MANAGER_H_
#define _SCREEN_MANAGER_H_

#include "Planner.h"
#include "ToDoManagement.h"

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <chrono>
#include <iomanip>
#include <string>
#include <thread>

#define ARROW 224
#define UP    72
#define DOWN  80
#define ENTER 13
#define ESC   27


/* 화면에 보여지는 모든 스크린을 관리하는 abstract 클래스
 * HomeScreen, EnterToDoScreen, LoadToDoScreen의 부모 클래스이다.
 * 자식 클래스는 반드시 drawScreen()을 override 해야 한다.
 */
class ScreenManager {
protected:
	// 버퍼 관련 멤버
	HANDLE hBuffer[2];
	bool active;
	CONSOLE_CURSOR_INFO cursor;

	// 플래너 및 To-do 관련 멤버
	Planner myPlanner;
	ToDoManagement myTdm;

public:
	ScreenManager() = default;
	ScreenManager(Planner& planner, ToDoManagement& tdm);

	virtual ~ScreenManager()	// 부모 클래스의 destructor를 virtual로 설정해 메모리 누수 방지
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

	void drawTitle(HANDLE handle);
	int backToHomeScreen();
};

#endif