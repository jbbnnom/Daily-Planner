#define _CRT_SECURE_NO_WARNINGS

#include "ToDoManagement.h"

using namespace std;
namespace fs = std::filesystem;

/*Index가 유효한지 간편하게 확인하기 위한 함수.다른 함수 내에서만 쓰임.*/
bool ToDoManagement::IndexExistence(int index)
{
	if (index >= 0 && index < todos.size()) {
		return true;
	}
	else {
		cerr << "Invalied index\n";
		return false;
	}
}

/* todo 목록 불러오기 */

//해당하는 일자의 todo목록 불러오기
void ToDoManagement::getToDosByDate(int date) {
	clearToDos();

	loadOneDayToDos(to_string(date));

}

//category에 해당하는 todo 불러오기
vector<ToDo> ToDoManagement::getToDoByCategory(string category) {
	vector<ToDo> result;
	for (auto& todo : todos) {
		if (todo.getCategory() == category) {
			result.push_back(todo);
		}
	}
	return result;
}

//importance에 따라 todo 정렬
vector<ToDo> ToDoManagement::sortToDoByImportance() {
	vector<ToDo> sortedTodos = todos;
	sort(sortedTodos.begin(), sortedTodos.end(), compareImportance);
	return sortedTodos;
}


/* 일 텍스트 파일과의 상호작용 */

//하나의 to-do를 파일에 저장
void ToDoManagement::save_toFile(fstream &file, ToDo &todo) {
	file << todo.getTask() << "," << todo.getDate() << ","
		<< todo.getStartTime() << "," << todo.getEndTime() << ","
		<< todo.getCategory() << "," << todo.getImportance() << ","
		<< todo.getCheck() << "\n";

}

//한 날짜의 todo 파일 불러오기
void ToDoManagement::loadOneDayToDos(const fs::path& filename) {	//파일이름은 날짜
	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Cannot open file: " << filename << endl;
		return;
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
}