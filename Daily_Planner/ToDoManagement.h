#ifndef _TO_DO_MANAGEMENT_H_
#define _TO_DO_MANAGEMENT_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include "ToDo.h"

class ToDoManagement {
private:
	std::vector<ToDo> todos;

	static bool compareImportance(ToDo& a, ToDo& b) {
		return a.getImportance() > b.getImportance();
	}

public:

	void clearToDos() { todos.clear(); }

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



	/* todo ��� �ҷ����� */

	//�ش��ϴ� ������ todo��� �ҷ�����
	void getToDosByDate(int date);

	//category�� �ش��ϴ� todo �ҷ�����
	std::vector<ToDo> getToDoByCategory(std::string category);

	//importance�� ���� todo ����
	std::vector<ToDo> sortToDoByImportance();


	/* �� �ؽ�Ʈ ���ϰ��� ��ȣ�ۿ� */

	//��¥���� ���Ͽ� ����
	static void save_toFile(std::fstream& file, ToDo &todo);

	//�� ��¥�� todo ���� �ҷ�����
	void loadOneDayToDos(const std::filesystem::path& filename);
};

#endif