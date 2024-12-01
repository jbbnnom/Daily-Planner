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
	// Constructor and Destructor declaration
	Planner() = default;
	~Planner()
	{
		delete ym;
		delete d;
		delete ymd;
	}

	// Member function declaration
	void printCalendar();
	void writeToFile(ToDo& todo);
	void loadAllToDos(ToDoManagement& tdm);
	void resetPlannerPath() { plannerPath = std::filesystem::path(getenv("USERPROFILE")) / "Desktop" / "Daily Planner"; }

	// Setter
	void setYearMonth(int yearValue, int monthValue);
	void setDay(int dayValue);
	void setYearMonthDay();
	void setPlannerPath(int mode);

	// Getter
	std::chrono::year_month_day getYearMonthDay() { return *ymd; }
};


std::string dateToStr(std::chrono::year_month_day ymd)
{
	return to_string(static_cast<int>(ymd.year())) +
		"-" + to_string(static_cast<unsigned>(ymd.month())) +
		"-" + to_string(static_cast<unsigned>(ymd.day()));
}

#endif