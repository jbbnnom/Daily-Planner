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

class Planner {
private:
	std::filesystem::path plannerPath = std::filesystem::path(getenv("USERPROFILE")) / "Desktop" / "Daily Planner";
	std::chrono::year_month *ym;
	std::chrono::day* day;
	std::vector<const char *> stringMonth = { "",
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	};
public:
	Planner() = default;
	~Planner()
	{
		delete ym;
		delete day;
	}

	
	void printCalendar();
	void writeToFile();
	void readToDo();

	void setYearMonth(int yearValue, int monthValue)
	{
		ym = new std::chrono::year_month{ std::chrono::year(yearValue) / std::chrono::month(monthValue) };
	}
	void setDay(int dayValue) { day = new std::chrono::day(dayValue); }
	void setPlannerPath();
};

#endif