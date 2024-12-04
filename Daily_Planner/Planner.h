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

/* ���� ���� ���, ��¥�� �����ϴ� Planner Ŭ����
 * ToDoManagement Ŭ������ ���� to-do�� Ȱ���Ѵ�.
 */
class Planner {
private:
	std::filesystem::path plannerPath = std::filesystem::path(getenv("USERPROFILE")) / "Desktop" / "Daily Planner";	// ����ȭ�� ���͸�
	std::chrono::year_month* ym = nullptr;
	std::chrono::day* d = nullptr;
	std::chrono::year_month_day* ymd = nullptr;
	std::vector<const char *> stringMonth = { "",
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	};

public:
	// Constructor �� Destructor ����
	Planner() = default;
	~Planner()
	{
		delete ym;
		delete d;
		delete ymd;
	}

	// ��� �Լ� ����
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

	// ��� �ʱ�ȭ
	void resetPlannerPath() { plannerPath = std::filesystem::path(getenv("USERPROFILE")) / "Desktop" / "Daily Planner"; }

	// Getter
	std::filesystem::path getPlannerPath() { return plannerPath; }
	std::chrono::year_month_day getYearMonthDay() { return *ymd; }
};

std::string dateToStr(std::chrono::year_month_day& ymd);

#endif