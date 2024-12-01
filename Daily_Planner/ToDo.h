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
	int importance = 0;		// 0�� �߿䵵�� ���� ����. (���� �ļ���)
	bool check = false;		// true�� ���� �Ϸ� ����. false�� ���� ���� �� �� ����.
public:
	//constructor
	ToDo(std::string task = "nothing", std::string date,
		std::string start_time, std::string end_time,
		std::string category = "none", int importance = 0,
		bool check = false);

	//setter
	void taskComplete() {	//���� �� �½�ũ ���� �ٲ�. ���� �½�ũ ���¸� '�Ϸ� -> �̿Ϸ�'�� �ϰ� ���� ���� �̰ɷ� �����ϸ� ��.
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
		//�ƴ� ���� �̰� �ؾߵǳ�???????�׳� ������� �ϴ� �ۺ� �����ϸ� �ȵǳ�
	}
	int getCheck() {
		return check;
	}

	static std::string now_date();
	static std::string now_time();
};

#endif