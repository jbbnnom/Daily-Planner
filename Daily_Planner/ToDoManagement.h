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



	/* todo 목록 불러오기 */

	//해당하는 일자의 todo목록 불러오기
	void getToDosByDate(int date);

	//category에 해당하는 todo 불러오기
	std::vector<ToDo> getToDoByCategory(std::string category);

	//importance에 따라 todo 정렬
	std::vector<ToDo> sortToDoByImportance();


	/* 일 텍스트 파일과의 상호작용 */

	//날짜별로 파일에 저장
	static void save_toFile(std::fstream& file, ToDo &todo);

	//한 날짜의 todo 파일 불러오기
	void loadOneDayToDos(const std::filesystem::path& filename);
};

#endif