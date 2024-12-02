#ifndef _TO_DO_MANAGEMENT_H_
#define _TO_DO_MANAGEMENT_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <regex>

#include "ToDo.h"

class ToDoManagement {
private:
	ToDo* newTodo;
	std::vector<ToDo> todos;
	std::vector<ToDo> searchResult;

	static bool compareImportance(ToDo& a, ToDo& b) {
		return a.getImportance() > b.getImportance();
	}

public:
	/* vector<ToDo> 내용 제거 */
	void clearToDos() { todos.clear(); searchResult.clear(); }

	/*Index가 유효한지 간편하게 확인하기 위한 함수.다른 함수 내에서만 쓰임.*/
	bool IndexExistence(int index);


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

	void setDate(int index, std::string date) {
		if (IndexExistence(index)) {
			todos[index].changeDate(date);
		}
	}

	void setStartTime(int index, std::string date) {
		if (IndexExistence(index)) {
			todos[index].changeStartTime(date);
		}
	}

	void setEndTime(int index, std::string date) {
		if (IndexExistence(index)) {
			todos[index].changeEndTime(date);
		}
	}

	void setCategory(int index, std::string category) {
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

	ToDo getNewToDo() { return *newTodo; }
	void deallocToDo() { delete newTodo; }
	std::vector<ToDo> getResult() { return searchResult; }

	bool isValidDate(const std::string& date);
	bool isValidTime(const std::string& time);

	/* 새로운 to-do 제작 */
	void makeNewToDo();


	/* todo 목록 불러오기 */
	int getToDosByDate(const std::filesystem::path& filename);
	void getToDoByCategory(const std::string &category);
	void sortToDoByImportance();


	/* 일 텍스트 파일과의 상호작용 */
	static void saveToDo(std::fstream& file, ToDo &todo);
	int loadOneDayToDos(const std::filesystem::path& filename);


	/* 각 불러오기 별 Todos vector 내 모든 요소 출력 */
	void printToDos_date();
	void printToDos_category();
	void printToDos_importance();
	ToDo printToDos_editMode();
};

#endif