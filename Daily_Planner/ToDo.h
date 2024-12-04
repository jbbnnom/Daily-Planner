#ifndef TODO_H
#define TODO_H

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <chrono>
#include <ctime>
#include <iostream>
#include <algorithm>


class ToDo {
private:
	std::string task = "nothing";
	std::string date;
	std::string start_time;
	std::string end_time = start_time;
	std::string category = "none";
	int importance = 0;		// 0은 중요도가 없는 상태. (가장 후순위)
	bool check = false;		// true는 이행 완료 상태. false는 아직 이행 안 된 상태.
public:
	//constructor
	ToDo(std::string task, std::string date,
		std::string start_time, std::string end_time,
		std::string category, int importance,
		bool check = false);

	//setter
	void taskComplete() {	//실행 시 태스크 상태 바뀜. 만약 태스크 상태를 '완료 -> 미완료'로 하고 싶을 때도 이걸로 실행하면 됨.
		check = !check;
	}
	void changeTask(std::string task) {
		this->task = task;
	}
	void changeCategory(std::string category) {
		this->category = category;
	}
	void changeImportance(int importance) {
		this->importance = importance;
	}
	void changeDate(std::string date) {
		this->date = date;
	}
	void changeStartTime(std::string time) {
		this->start_time = time;
	}
	void changeEndTime(std::string time) {
		this->end_time = time;
	}

	//getter
	std::string getTask() {
		return task;
	}
	std::string getDate() {
		return date;
	}
	std::string getStartTime() {
		return start_time;
	}
	std::string getEndTime() {
		return end_time;
	}
	std::string getCategory() {
		return category;
	}
	int getImportance() {
		return importance;
	}
	std::string getCheck() {
		return check ? "Y" : "N";
	}
};

#endif