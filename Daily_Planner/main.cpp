#define _CRT_SECURE_NO_WARNINGS

#include "ScreenManager.h"
#include "HomeScreen.h"
#include "EnterToDoScreen.h"
#include "LoadToDoScreen.h"

#include "Planner.h"

#include "ToDoManagement.h"
#include "ToDo.h"

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <filesystem>
#include <string>
#include <vector>


#define HOME  0
#define ENTER 1
#define LOAD  2

using namespace std;
namespace fs = std::filesystem;


int main()
{
	int mode = 0;
	Planner myPlanner;
	ToDoManagement myTdm;

	ScreenManager* screenMgr = new HomeScreen(myPlanner, myTdm);	// Dynamic binding을 통해 화면을 유동적으로 관리

	// 바탕화면 경로 지정 후 to-do가 저장될 폴더(디렉터리) 생성
	fs::path plannerDir = fs::path(getenv("USERPROFILE")) / "Desktop" / "Daily Planner";
	if (!fs::exists(plannerDir)) {
		fs::create_directory(plannerDir);
	}

	while (true) {
		screenMgr->drawScreen(mode);

		delete screenMgr;

		switch (mode) {
		case HOME:
			screenMgr = new HomeScreen(myPlanner, myTdm);
			break;
		case ENTER:
			screenMgr = new EnterToDoScreen(myPlanner, myTdm);
			break;
		case LOAD:
			screenMgr = new LoadToDoScreen(myPlanner, myTdm);
			break;
		}
	}
	
	delete screenMgr;
	
	return 0;
}