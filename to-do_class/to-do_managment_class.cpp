#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "ToDo.h"

using namespace std;

class ToDoManagement {
private:
	vector<ToDo> todos;

	static bool compareImportance(ToDo& a, ToDo& b) {
		return a.getImportance() > b.getImportance();
	}

public:

	/*Index�� ��ȿ���� �����ϰ� Ȯ���ϱ� ���� �Լ�.�ٸ� �Լ� �������� ����.*/
	bool IndexExistence(int index) {
		if (index >= 0 && index < todos.size()) {
			return true;
		}
		else {
			cerr << "Invalied index\n";
			return false;
		}
	}


	/* todo ������Ʈ �߰� & ���� & ���� */

	//todo�߰�
	void addToDo(const ToDo& todo) {
		todos.push_back(todo);
	}
	//todo����
	void editToDo(int index, const ToDo& todo) {
		if (IndexExistence(index)) {
			todos[index] = todo;
		}
	}
	//todo����
	void deleteToDo(int index) {
		if (IndexExistence(index)) {
			todos.erase(todos.begin() + index);
		}
	}


	/* todo�ȿ� ��� ���(����) */

	void setDate(int index, string date) {
		if (IndexExistence(index)) {
			todos[index].changeDate(date);
		}
	}

	void setStartTime(int index, string date) {
		if (IndexExistence(index)) {
			todos[index].changeStartTime(date);
		}
	}

	void setEndTime(int index, string date) {
		if (IndexExistence(index)) {
			todos[index].changeEndTime(date);
		}
	}

	void setCategory(int index, string category) {
		if (IndexExistence(index)) {
			todos[index].changeCategory(category);
		}
	}

	void setImportance(int index, int importance) {
		if (IndexExistence(index)) {
			todos[index].changeImportance(importance);
		}
	}

	void setCheck(int index) {
		if (IndexExistence(index)) {
			todos[index].taskComplete();
		}
	}



	/* todo ��� �ҷ����� */

	//�ش��ϴ� ������ todo��� �ҷ�����
	vector<ToDo> getToDosByDate(int date) {
		//�ϴ� �����ϰ� �ҷ�����..
		save_toFile();
		todos.clear();
		todos = loadToDos(to_string(date));

		return todos;
	}

	//category�� �ش��ϴ� todo �ҷ�����
	vector<ToDo> getToDoByCategory(string category) {
		vector<ToDo>result;
		for (auto& todo : todos) {
			if (todo.getCategory() == category) {
				result.push_back(todo);
			}
		}
		return result;
	}

	//importance�� ���� todo ����
	vector<ToDo> sortToDoByImportance() {
		vector<ToDo> sortedTodos = todos;
		sort(sortedTodos.begin(), sortedTodos.end(), compareImportance);
		return sortedTodos;
	}

	
	/* �� �ؽ�Ʈ ���ϰ��� ��ȣ�ۿ� */

	//��¥���� ���Ͽ� ����
	void save_toFile() {
		for (int i = 0; i < todos.size(); i++) {
			string filename = todos[i].getDate();

			ofstream file(filename);
			if (file.is_open()) {
				for (auto& todo : todos) {
					file << todo.getTask() << "," << todo.getDate() << ","
						<< todo.getStartTime() << "," << todo.getEndTime() << ","
						<< todo.getCategory() << "," << todo.getImportance() << ","
						<< todo.getCheck() << "\n";
				}
			}
		}
	}

	//�� ��¥�� todo ���� �ҷ�����
	vector<ToDo> loadToDos(const string& filename) {	//�����̸��� ��¥
		ifstream file(filename);
		vector<ToDo> todos;

		if (!file.is_open()) {
			cerr << "������ �� �� �����ϴ�: " << filename << endl;
			return todos;
		}

		string line;
		while (getline(file, line)) {
			string task, category, date, start_time, end_time;
			string token;
			int  importance;
			bool check;

			stringstream ss(line);

			// Parse each field from the line
			getline(ss, task, ',');
			getline(ss, token, ','); date;
			getline(ss, token, ','); start_time;
			getline(ss, token, ','); end_time;
			getline(ss, category, ',');
			getline(ss, token, ','); importance = stoi(token);
			getline(ss, token, ','); check = (token == "true");

			// Create a ToDo object and add it to the list
			todos.emplace_back(task, date, start_time, end_time, category, importance, check);
		}

		file.close();
		return todos;
	}

};