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

	/*Index가 유효한지 간편하게 확인하기 위한 함수.다른 함수 내에서만 쓰임.*/
	bool IndexExistence(int index) {
		if (index >= 0 && index < todos.size()) {
			return true;
		}
		else {
			cerr << "Invalied index\n";
			return false;
		}
	}


	/* todo 오브젝트 추가 & 수정 & 삭제 */

	//todo추가
	void addToDo(const ToDo& todo) {
		todos.push_back(todo);
	}
	//todo수정
	void editToDo(int index, const ToDo& todo) {
		if (IndexExistence(index)) {
			todos[index] = todo;
		}
	}
	//todo삭제
	void deleteToDo(int index) {
		if (IndexExistence(index)) {
			todos.erase(todos.begin() + index);
		}
	}


	/* todo안에 요소 등록(변경) */

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



	/* todo 목록 불러오기 */

	//해당하는 일자의 todo목록 불러오기
	vector<ToDo> getToDosByDate(int date) {
		//일단 저장하고 불러오기..
		save_toFile();
		todos.clear();
		todos = loadToDos(to_string(date));

		return todos;
	}

	//category에 해당하는 todo 불러오기
	vector<ToDo> getToDoByCategory(string category) {
		vector<ToDo>result;
		for (auto& todo : todos) {
			if (todo.getCategory() == category) {
				result.push_back(todo);
			}
		}
		return result;
	}

	//importance에 따라 todo 정렬
	vector<ToDo> sortToDoByImportance() {
		vector<ToDo> sortedTodos = todos;
		sort(sortedTodos.begin(), sortedTodos.end(), compareImportance);
		return sortedTodos;
	}

	
	/* 일 텍스트 파일과의 상호작용 */

	//날짜별로 파일에 저장
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

	//한 날짜의 todo 파일 불러오기
	vector<ToDo> loadToDos(const string& filename) {	//파일이름은 날짜
		ifstream file(filename);
		vector<ToDo> todos;

		if (!file.is_open()) {
			cerr << "파일을 열 수 없습니다: " << filename << endl;
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