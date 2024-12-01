#define _CRT_SECURE_NO_WARNINGS

#include "ToDoManagement.h"

using namespace std;
namespace fs = std::filesystem;

/*Index�� ��ȿ���� �����ϰ� Ȯ���ϱ� ���� �Լ�.�ٸ� �Լ� �������� ����.*/
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

/* todo ��� �ҷ����� */

//�ش��ϴ� ������ todo��� �ҷ�����
void ToDoManagement::getToDosByDate(int date) {
	clearToDos();

	loadOneDayToDos(to_string(date));

}

//category�� �ش��ϴ� todo �ҷ�����
vector<ToDo> ToDoManagement::getToDoByCategory(string category) {
	vector<ToDo> result;
	for (auto& todo : todos) {
		if (todo.getCategory() == category) {
			result.push_back(todo);
		}
	}
	return result;
}

//importance�� ���� todo ����
vector<ToDo> ToDoManagement::sortToDoByImportance() {
	vector<ToDo> sortedTodos = todos;
	sort(sortedTodos.begin(), sortedTodos.end(), compareImportance);
	return sortedTodos;
}


/* �� �ؽ�Ʈ ���ϰ��� ��ȣ�ۿ� */

//�ϳ��� to-do�� ���Ͽ� ����
void ToDoManagement::save_toFile(fstream &file, ToDo &todo) {
	file << todo.getTask() << "," << todo.getDate() << ","
		<< todo.getStartTime() << "," << todo.getEndTime() << ","
		<< todo.getCategory() << "," << todo.getImportance() << ","
		<< todo.getCheck() << "\n";

}

//�� ��¥�� todo ���� �ҷ�����
void ToDoManagement::loadOneDayToDos(const fs::path& filename) {	//�����̸��� ��¥
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