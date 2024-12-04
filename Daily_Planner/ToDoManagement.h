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
	/* �� vector<ToDo> ���� ���� */
	void clearToDos() { todos.clear(); searchResult.clear(); }

	ToDo getNewToDo() { return *newTodo; }
	void deallocToDo() { delete newTodo; }
	std::vector<ToDo> getResult() { return searchResult; }
	std::vector<ToDo> getToDos() { return todos; }

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
	void changeComplete(ToDo& todo);
};

#endif