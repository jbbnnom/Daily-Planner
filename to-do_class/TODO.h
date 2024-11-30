#ifndef TODO_H
#define TODO_H

#define _CRT_SECURE_NO_WARNINGS

#include <string>


using namespace std;


class ToDo {
private:
	string task = "nothing";
	string date;	
	string start_time;	
	string end_time = start_time;
	string category = "none";
	int importance = 0;
	bool check = false;
public:
	//constructor
	ToDo(string task = "nothing", int date = 0 ,
		int start_time = 0, int end_time = 0,
		string category = "none", int importance = 0,
		bool check = false);

	//setter
	void taskComplete();
	void changeTask(string task);
	void changeCategory(string category);
	void changeImportance(int importance);
	void changeDate(string date);
	void changeStartTime(string time);
	void changeEndTime(string time);

	//getter
	string getTask() {
		return task;
	}
	string getDate() {
		return date;
	}
	string getStartTime() {
		return start_time;
	}
	string getEndTime() {
		return end_time;
	}
	string getCategory() {
		return category;
	}
	int getImportance() {
		return importance;
		//�ƴ� ���� �̰� �ؾߵǳ�???????�׳� ������� �ϴ� �ۺ� �����ϸ� �ȵǳ�
	}
	int getCheck() {
		return check;
	}

	static int now_date();
	static int now_time();
};

#endif