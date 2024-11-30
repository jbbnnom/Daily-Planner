#define _CRT_SECURE_NO_WARNINGS

#include "ScreenManager.h"
#include "HomeScreen.h"
#include "PlannerScreen.h"

#include "Planner.h"

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <filesystem>
#include <string>
#include <vector>


#define HOME 0
#define PL 1

using namespace std;
namespace fs = std::filesystem;

int main()
{
	int mode = 0;
	ScreenManager* screenMgr = new HomeScreen();	// Dynamic binding을 통해 화면을 유동적으로 관리
	Planner myPlanner{};

	// 바탕화면 경로 지정 후 할 일이 저장될 폴더(디렉터리) 생성
	fs::path plannerDir = fs::path(getenv("USERPROFILE")) / "Desktop" / "Daily Planner";
	if (!fs::exists(plannerDir)) {
		fs::create_directory(plannerDir);
	}

	while (true) {
		screenMgr->drawScreen(mode);

		delete screenMgr;

		switch (mode) {
		case HOME:
			continue;
		case PL:
			screenMgr = new PlannerScreen(myPlanner);
		}
	}
	
	return 0;
}