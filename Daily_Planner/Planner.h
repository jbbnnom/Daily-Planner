#define _CRT_SECURE_NO_WARNINGS

#ifndef _PLANNER_H_
#define _PLANNER_H_

#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <vector>
#include <filesystem>
#include <format>
#include <fstream>

#include "ToDoManagement.h"

/* 파일 저장 경로, 날짜를 관리하는 Planner 클래스
 * ToDoManagement 클래스를 통해 to-do를 활용한다.
 */
class Planner {
private:
	std::filesystem::path plannerPath = std::filesystem::path(getenv("USERPROFILE")) / "Desktop" / "Daily Planner";	// 바탕화면 디렉터리
	std::chrono::year_month* ym = nullptr;
	std::chrono::day* d = nullptr;
	std::chrono::year_month_day* ymd = nullptr;
	std::vector<const char *> stringMonth = { "",
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	};

public:
	// Constructor 및 Destructor 선언
	Planner() = default;
	~Planner()
	{
		delete ym;
		delete d;
		delete ymd;
	}

	// 멤버 함수 선언
	void printCalendar(int mode);
	void writeToFile(ToDo& todo);
	void loadAllToDos(ToDoManagement& tdm);
	void overlapToDos(ToDoManagement& tdm);

	// Setter
	void setPlannerPath(int mode);
	void setYearMonth(int yearValue, int monthValue);
	void setDay(int dayValue);
	void setYearMonthDay();
	void setYearMonthDay(int yearValue, int monthValue, int dayValue);
	void setYearMonthDay(std::chrono::year_month_day& ymd);

	// 경로 초기화
	void resetPlannerPath() { plannerPath = std::filesystem::path(getenv("USERPROFILE")) / "Desktop" / "Daily Planner"; }

	// Getter
	std::filesystem::path getPlannerPath() { return plannerPath; }
	std::chrono::year_month_day getYearMonthDay() { return *ymd; }
};

std::string dateToStr(std::chrono::year_month_day& ymd);

#endif