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
#define WRITE 1
#define READ  2

using namespace std;
namespace fs = std::filesystem;


int main()
{
	int mode = 0;   // ȭ�� ��� (0: HOME, 1: EnterToDOScreen, 2: LoadToDoScreen)
	Planner myPlanner;
	ToDoManagement myTdm;

	ScreenManager* screenMgr = new HomeScreen(myPlanner, myTdm);	// Dynamic binding�� ���� ȭ���� ���������� ����

	// ����ȭ�� ��� ���� �� to-do�� ����� ����(���͸�) ����
	fs::path plannerDir = fs::path(getenv("USERPROFILE")) / "Desktop" / "Daily Planner";
	if (!fs::exists(plannerDir)) {
		fs::create_directory(plannerDir);
	}

    while (true) {
        // ���� ȭ�� ���
        screenMgr->drawScreen(mode);

        // ȭ�� ��� ���� �� ���� ȭ�� ��� ����
        if (mode == 0) {
            delete screenMgr;
            screenMgr = new HomeScreen(myPlanner, myTdm);
            continue;
        }

        delete screenMgr;
        screenMgr = nullptr;

        switch (mode) {
        case WRITE:
            screenMgr = new EnterToDoScreen(myPlanner, myTdm);
            break;
        case READ:
            screenMgr = new LoadToDoScreen(myPlanner, myTdm);
            break;
        }
    }  // end of while

	delete screenMgr;
	return 0;
}