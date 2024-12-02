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
	/* vector<ToDo> ���� ���� */
	void clearToDos() { todos.clear(); searchResult.clear(); }

	/*Index�� ��ȿ���� �����ϰ� Ȯ���ϱ� ���� �Լ�.�ٸ� �Լ� �������� ����.*/
	bool IndexExistence(int index);


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

	/* ���ο� to-do ���� */
	void makeNewToDo();


	/* todo ��� �ҷ����� */
	int getToDosByDate(const std::filesystem::path& filename);
	void getToDoByCategory(const std::string &category);
	void sortToDoByImportance();


	/* �� �ؽ�Ʈ ���ϰ��� ��ȣ�ۿ� */
	static void saveToDo(std::fstream& file, ToDo &todo);
	int loadOneDayToDos(const std::filesystem::path& filename);


	/* �� �ҷ����� �� Todos vector �� ��� ��� ��� */
	void printToDos_date();
	void printToDos_category();
	void printToDos_importance();
	ToDo printToDos_editMode();
};

#endif