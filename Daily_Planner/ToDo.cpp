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