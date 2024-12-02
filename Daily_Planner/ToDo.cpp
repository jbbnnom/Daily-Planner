#define _CRT_SECURE_NO_WARNINGS

#include "ToDo.h"

using namespace std;
using namespace std::chrono;


// Constructor definition
ToDo::ToDo(string task, string date,
	string start_time, string end_time,
	string category, int importance,
	bool check)
{
	this->task = task;
	this->date = date;
	this->start_time = start_time;
	this->end_time = end_time;
	this->category = category;
	this->importance = importance;
	this->check = check;
}


static string now_date() {
	auto now = system_clock::now();
	time_t nowTime = system_clock::to_time_t(now);
	tm localTime = *localtime(&nowTime);
	char dateBuffer[11];
	strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &localTime);

	return string(dateBuffer);
}

static string now_time() {
	auto now = system_clock::now();
	time_t nowTime = system_clock::to_time_t(now);
	tm localTime = *localtime(&nowTime);
	char timeBuffer[9];
	strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &localTime);

	return string(timeBuffer);
}