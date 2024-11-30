#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <chrono>
#include <ctime>
#include <iostream>
#include <algorithm>
#include "TODO.h"

using namespace std;
using namespace std::chrono;



class ToDo {

private:
	string task;
	string date;	
	string start_time;	
	string end_time;
	string category;
	int importance;	//0은 중요도가 없는 상태. (가장 후순위)
	bool check;		//true는 이행 완료 상태. false는 아직 이행 안 된 상태.



public:
	//constructor
	
	ToDo(string task, string date,
		string start_time, string end_time, 
		string category = "none", int importance = 0,
		bool check = false) {

		this->task = task;
		this->date = date;
		this->start_time = start_time;
		this->end_time = end_time;
		this->category = category;
		this->importance = importance;
		this->check = check;
	}

	//setter
	void taskComplete() {	//실행 시 태스크 상태 바뀜. 만약 태스크 상태를 '완료 -> 미완료'로 하고 싶을 때도 이걸로 실행하면 됨.
		check = !check;
	}
	void changeTask(string task) {
		this->task = task;
	}
	void changeCategory(string category) {
		this->category = category;
	}
	void changeImportance(int importance) {
		this->importance = importance;
	}
	void changeDate(string date) {
		this->date = date;
	}
	void changeStartTime(string time) {
		this->start_time = time;
	}
	void changeEndTime(string time) {
		this->end_time = time;
	}

	



	string now_date() {	
		auto now = system_clock::now();
		time_t nowTime = system_clock::to_time_t(now);
		tm localTime = *localtime(&nowTime);
		char dateBuffer[11];
		strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &localTime);

		return string(dateBuffer);
	}

	string now_time() {	
		auto now = system_clock::now();
		time_t nowTime = system_clock::to_time_t(now);
		tm localTime = *localtime(&nowTime);
		char timeBuffer[9];
		strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &localTime);

		return string(timeBuffer);
	}

};