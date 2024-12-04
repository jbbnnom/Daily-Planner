#define _CRT_SECURE_NO_WARNINGS

#include "ToDoManagement.h"

using namespace std;
namespace fs = std::filesystem;

/* to-do�� �Է��� ��¥ ������ ��ȿ�� Ȯ�� [YYYY/MM/DD] */
bool ToDoManagement::isValidDate(const string& date)
{
	// �Է¹��� ��¥ ���ڿ��� ���� ǥ�������� ��ȯ
	regex dateFormat(R"(\d{4}-\d{2}-\d{2})");

	if (regex_match(date, dateFormat)) {
		// ������ ������ ���� ���� ���� �˻�
		int year, month, day;
		if (sscanf(date.c_str(), "%4d-%2d-%2d", &year, &month, &day) == 3) {
			if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
				return true;
			}
		}
	}
	return false;
}

/* to-do�� �Է��� �ð� ������ ��ȿ�� Ȯ�� [HH:MM:SS] */
bool ToDoManagement::isValidTime(const string& time)
{
	// �Է¹��� �ð� ���ڿ��� ���� ǥ�������� ��ȯ
	regex timeFormat(R"(\d{2}:\d{2}:\d{2})");

	if (regex_match(time, timeFormat)) {
		// ������ ������ �ÿ� ��, ���� ���� �˻�
		int hour, minute, second;
		if (sscanf(time.c_str(), "%2d:%2d:%2d", &hour, &minute, &second) == 3) {
			if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59 && second >= 0 && second <= 59) {
				return true;
			}
		}
	}
	return false;
}

/* ����ڷκ��� �ϳ��� to-do�� ���� ������ �Է¹޾� newTodo ����� ���� */
void ToDoManagement::makeNewToDo()
{
	string task, date, startTime, endTime, category;
	int importance;

	cout << "---------------Enter the information about your To-do----------------" << endl;

	cout << "Enter task: ";
	if (cin.peek() == '\n') {
		cin.ignore();
	}
	getline(cin, task);

	while (true) {
		cout << "Enter start time [HH:MM:SS]: ";
		getline(cin, startTime);

		if (!isValidTime(startTime)) {
			cout << "Invalid time format. Try again." << endl;
		}
		else {
			break;
		}
	}

	while (true) {
		cout << "Enter end time [HH:MM:SS]: ";
		getline(cin, endTime);

		if (!isValidTime(endTime)) {
			cout << "Invalid time format. Try again." << endl;
		}
		else {
			break;
		}
	}

	cout << "Enter category: ";
	getline(cin, category);

	cout << "Enter importance: ";
	cin >> importance;

	newTodo = new ToDo(task, date, startTime, endTime, category, importance);	// check�� default parameter -> �׻� false
}


//�ش��ϴ� ������ todo��� �ҷ�����
int ToDoManagement::getToDosByDate(const fs::path& filename) {
	int code;	// ���� ����ڰ� �Է��� ��¥�� �ش��ϴ� to-do list�� ���� ��� 1�� �����

	clearToDos();
	if (code = loadOneDayToDos(filename)) {
		return 1;
	}
	return 0;
}

//category�� �ش��ϴ� todo �ҷ�����
void ToDoManagement::getToDoByCategory(const string& category) {
	for (auto& todo : todos) {
		if (todo.getCategory() == category) {
			searchResult.push_back(todo);
		}
	}
}

//importance�� ���� todo ����
void ToDoManagement::sortToDoByImportance() {
	sort(todos.begin(), todos.end(), compareImportance);
}


//�ϳ��� to-do�� ���Ͽ� ����
void ToDoManagement::saveToDo(fstream &file, ToDo &todo) {
	file << todo.getTask() << "," << todo.getDate() << ","
		 << todo.getStartTime() << "," << todo.getEndTime() << ","
		 << todo.getCategory() << "," << todo.getImportance() << ","
		 << todo.getCheck() << "\n";
}

/* �� ��¥�� todo ���� �ҷ����� 
 * (��Ÿ�� ���� �߻��ؼ� �ٽ� �ۼ��߽��ϴ�)
 */
int ToDoManagement::loadOneDayToDos(const fs::path& filename) {
	ifstream file(filename);

	if (!file.is_open()) {	// �Է��� ��¥�� to-do list�� ���� ���
		cerr << "\nThere is no to-do list in ";	// �� Ŭ������ ��¥ ������ ������ �� ���� ����ؼ� ���� �ڵ� ��ȯ
		return 1;
	}

	string line;
	while (getline(file, line)) {
		string task, category, date, start_time, end_time;
		string token;
		int importance = 0;
		bool check = false;

		stringstream ss(line);

		try {
			// �� ���� �� �ʵ� parsing
			getline(ss, task, ',');
			getline(ss, date, ',');
			getline(ss, start_time, ',');
			getline(ss, end_time, ',');
			getline(ss, category, ',');

			// �߿䵵�� ������ ���� token���� ����
			getline(ss, token, ',');
			if (!token.empty()) {
				importance = stoi(token); // �� �������� ���� �߻� ���ɼ� ����
			}

			// �Ϸ� ���� ���� ���� ����
			getline(ss, token, ',');
			if (!token.empty()) {
				check = (token == "Y");
			}

			// ToDo ��ü�� ����� todos ���Ϳ� ����
			// emplace_back�� ����� �Ű��������� ������������ �ڵ����� ��ü�� ��������
			todos.emplace_back(task, date, start_time, end_time, category, importance, check);

		}
		catch (const std::exception& e) {
			// Ȥ�ø� ���� �߻� ���ɼ� ���
			cerr << "Error parsing line: " << line << ". Error: " << e.what() << endl;
			continue;
		}
	}

	file.close();
	return 0;	// ���� ����
}


/* ����ڰ� "Load by date" ���� �� ��� ��� */
void ToDoManagement::printToDos_date()
{
	cout << setw(10) << left << "Category"
		 << setw(12) << left << "Importance"
		 << setw(27) << left << "Task"
		 << setw(12) << left << "Start time"
		 << setw(10) << left << "End time"
		 << setw(9) << left << "Completed";
	cout << "--------------------------------------------------------------------------------\n";

	for (auto& todo : todos)
	{
		cout << setw(10) << left << todo.getCategory()
			 << setw(12) << left << todo.getImportance()
			 << setw(27) << left << todo.getTask()
			 << setw(12) << left << todo.getStartTime()
			 << setw(10) << left << todo.getEndTime()
			 << setw(9) << left << todo.getCheck() << endl;
	}
	cout << "\n\n";
}

/* ����ڰ� "Load by category" ���� �� ��� ��� */
void ToDoManagement::printToDos_category()
{
	cout << setw(12) << left << "Date"
		 << setw(12) << left << "Importance"
		 << setw(27) << left << "Task"
		 << setw(12) << left << "Start time"
		 << setw(10) << left << "End time"
		 << setw(9) << left << "Completed";
	cout << "----------------------------------------------------------------------------------\n";

	for (auto& todo : searchResult)
	{
		cout << setw(12) << left << todo.getDate()
			 << setw(12) << left << todo.getImportance()
			 << setw(27) << left << todo.getTask()
			 << setw(12) << left << todo.getStartTime()
			 << setw(10) << left << todo.getEndTime()
			 << setw(9) << left << todo.getCheck() << endl;
	}
	cout << "\n";
}

/* ����ڰ� "Load after sorting by importance" ���� �� ��� ��� */
void ToDoManagement::printToDos_importance()
{
	cout << setw(12) << left << "Date"
		 << setw(10) << left << "Category"
		 << setw(12) << left << "Importance"
		 << setw(27) << left << "Task"
		 << setw(12) << left << "Start time"
		 << setw(10) << left << "End time"
		 << setw(9) << left << "Completed";
	cout << "--------------------------------------------------------------------------------------------\n";

	for (auto& todo : todos)
	{
		cout << setw(12) << left << todo.getDate()
			 << setw(10) << left << todo.getCategory()
			 << setw(12) << left << todo.getImportance()
			 << setw(27) << left << todo.getTask()
			 << setw(12) << left << todo.getStartTime()
			 << setw(10) << todo.getEndTime()
			 << setw(9) << left << todo.getCheck() << endl;
	}
	cout << "\n";
}

/* ����ڰ� ���ϴ� to-do�� �Ϸ� ���θ� �����ϰ� ����� �Լ� */
ToDo ToDoManagement::printToDos_editMode()
{
	int userInput, i;
	vector<ToDo> tempVec = searchResult.empty() ? todos : searchResult;	// �� �� ������� ���� ���Ͱ� ���� ����ڰ� �Է��� ������ ��� ����

	cout << setw(5) << left << "No."
		 << setw(12) << left << "Date"
		 << setw(10) << left << "Category"
	 	 << setw(12) << left << "Importance"
	 	 << setw(27) << left << "Task"
		 << setw(12) << left << "Start time"
		 << setw(10) << left << "End time"
		 << setw(9) << left << "Completed";
	cout << "-------------------------------------------------------------------------------------------------\n";

	for (i = 0; i != tempVec.size(); i++)
	{
		cout << setw(5) << left << i + 1
			 << setw(12) << left << tempVec[i].getDate()
			 << setw(10) << left << tempVec[i].getCategory()
			 << setw(12) << left << tempVec[i].getImportance()
			 << setw(27) << left << tempVec[i].getTask()
			 << setw(12) << left << tempVec[i].getStartTime()
			 << setw(10) << left << tempVec[i].getEndTime()
			 << setw(9) << left << tempVec[i].getCheck() << endl;
	}
	cout << "\n";

	// ����ڷκ��� �Ϸ� ���θ� �����ϰ��� �ϴ� ToDo�� ��ȣ�� �Է¹޾� �ش� ToDo ��ü�� ����
	while (true) {
		cout << "Enter a number of to-do you want to edit: ";
		cin >> userInput;

		if (userInput < 1 || userInput > tempVec.size()) {
			cout << "Invalid input. Try again." << endl;
			continue;
		}
		break;
	}

	return tempVec[userInput - 1];
}

/* �Ű������� �־��� to-do�� list ������ ã�� �Ϸ� ���θ� �����ϴ� �Լ� */
void ToDoManagement::changeComplete(ToDo& todo)
{
	for (auto it = todos.begin(); it != todos.end(); it++) {
		if (it->getTask() == todo.getTask()) {
			it->taskComplete();
			return;
		}
	}
}
