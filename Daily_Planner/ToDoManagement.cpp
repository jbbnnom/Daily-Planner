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


/* to-do에 입력한 날짜 포맷의 유효성 확인 [YYYY/MM/DD] */
bool ToDoManagement::isValidDate(const string& date)
{
	// 입력받은 날짜 문자열을 정규 표현식으로 변환
	regex dateFormat(R"(\d{4}-\d{2}-\d{2})");

	if (regex_match(date, dateFormat)) {
		// 포맷이 맞으면 월과 일의 범위 검사
		int year, month, day;
		if (sscanf(date.c_str(), "%4d-%2d-%2d", &year, &month, &day) == 3) {
			if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
				return true;
			}
		}
	}
	return false;
}

/* to-do에 입력한 시간 포맷의 유효성 확인 [HH:MM:SS] */
bool ToDoManagement::isValidTime(const string& time)
{
	// 입력받은 시간 문자열을 정규 표현식으로 변환
	regex timeFormat(R"(\d{2}:\d{2}:\d{2})");

	if (regex_match(time, timeFormat)) {
		// 포맷이 맞으면 시와 분, 초의 범위 검사
		int hour, minute, second;
		if (sscanf(time.c_str(), "%2d:%2d:%2d", &hour, &minute, &second) == 3) {
			if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59 && second >= 0 && second <= 59) {
				return true;
			}
		}
	}
	return false;
}

/* 사용자로부터 하나의 to-do에 대한 정보를 입력받아 newTodo 멤버에 저장 */
void ToDoManagement::makeNewToDo()
{
	string task, date, startTime, endTime, category;
	int importance;

	cout << "----------Enter the information about your To-do----------" << endl;

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

	newTodo = new ToDo(task, date, startTime, endTime, category, importance);	// check는 default parameter -> 항상 false
}


//해당하는 일자의 todo목록 불러오기
int ToDoManagement::getToDosByDate(const fs::path& filename) {
	int code;	// 만약 사용자가 입력한 날짜에 해당하는 to-do list가 없는 경우 1이 저장됨

	clearToDos();
	if (code = loadOneDayToDos(filename)) {
		return 1;
	}
	return 0;
}

//category에 해당하는 todo 불러오기
void ToDoManagement::getToDoByCategory(const string& category) {
	cout << todos.size() << endl;
	for (auto& todo : todos) {
		if (todo.getCategory() == category) {
			searchResult.push_back(todo);
		}
	}
}

//importance에 따라 todo 정렬
void ToDoManagement::sortToDoByImportance() {
	sort(todos.begin(), todos.end(), compareImportance);
}


//하나의 to-do를 파일에 저장
void ToDoManagement::saveToDo(fstream &file, ToDo &todo) {
	file << todo.getTask() << "," << todo.getDate() << ","
		 << todo.getStartTime() << "," << todo.getEndTime() << ","
		 << todo.getCategory() << "," << todo.getImportance() << ","
		 << todo.getCheck() << "\n";
}

/* 한 날짜의 todo 파일 불러오기 
 * (런타임 에러 발생해서 다시 작성했습니다)
 */
int ToDoManagement::loadOneDayToDos(const fs::path& filename) {
	ifstream file(filename);

	if (!file.is_open()) {	// 입력한 날짜에 to-do list가 없는 경우
		cerr << "There is no to-do list in ";	// 이 클래스에 날짜 정보를 
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
			// Parse each field from the line
			getline(ss, task, ',');
			getline(ss, date, ',');
			getline(ss, start_time, ',');
			getline(ss, end_time, ',');
			getline(ss, category, ',');

			// Parse and validate importance
			getline(ss, token, ',');
			if (!token.empty()) {
				importance = stoi(token); // May throw exception
			}

			// Parse and validate check
			getline(ss, token, ',');
			if (!token.empty()) {
				check = (token == "true");
			}

			// Create a ToDo object and add it to the list
			todos.emplace_back(task, date, start_time, end_time, category, importance, check);

		}
		catch (const std::exception& e) {
			cerr << "Error parsing line: " << line << ". Error: " << e.what() << endl;
			// Skip this line and continue with the next one
			continue;
		}
	}

	file.close();
	return 0;
}


/* 사용자가 "Load by date" 선택 시 결과 출력 */
void ToDoManagement::printToDos_date()
{
	cout << setw(12) << left << "Importance"
		 << setw(10) << left << "Category"
		 << setw(20) << left << "Task"
		 << setw(11) << left << "Start time"
		 << setw(8) << "End time"
		 << setw(11) << "Completed" << endl;

	for (auto& todo : todos)
	{
		cout << setw(12) << todo.getImportance()
			 << setw(10) << left << todo.getCategory()
			 << setw(20) << left << todo.getTask()
			 << setw(11) << left << todo.getStartTime()
			 << setw(8) << todo.getEndTime()
			 << setw(4) << left << todo.getCheck() << endl;
	}
	cout << "\n";
}

/* 사용자가 "Load by category" 선택 시 결과 출력 */
void ToDoManagement::printToDos_category()
{
	cout << setw(15) << left << "Date" << " "
		 << setw(5) << left << "Importance" << " "
		 << setw(25) << left << "Task" << " "
		 << setw(12) << "Start time" << " " << setw(12) << "End time" << " "
		 << setw(15) << "Complete or not" << endl;

	for (auto& todo : searchResult)
	{
		cout << setw(5) << left << todo.getDate() << " "
			 << setw(5) << todo.getImportance() << " "
			 << setw(24) << todo.getTask() << " "
			 << setw(10) << todo.getStartTime() << " " << setw(12) << todo.getEndTime() << " "
			 << setw(12) << left << todo.getCheck() << endl;
	}
	cout << "\n";
}

/* 사용자가 "Load after sorting by importance" 선택 시 결과 출력 */
void ToDoManagement::printToDos_importance()
{
	cout << setw(5) << left << "Importance" << " "
	     << setw(15) << left << "Category" << " "
		 << setw(15) << left << "Date" << " "
		 << setw(25) << left << "Task" << " "
		 << setw(12) << "Start time" << " " << setw(12) << "End time" << " "
		 << setw(15) << "Complete or not" << endl;

	for (auto& todo : todos)
	{
		cout << setw(5) << left << todo.getImportance() << " "
			 << setw(15) << todo.getCategory() << " "
			 << setw(15) << left << todo.getDate() << " "
			 << setw(24) << todo.getTask() << " "
			 << setw(10) << todo.getStartTime() << " " << setw(12) << todo.getEndTime() << " "
			 << setw(12) << left << todo.getCheck() << endl;
	}
	cout << "\n";
}

/* 임시 */
ToDo ToDoManagement::printToDos_editMode()
{
	int userInput, i;
	vector<ToDo> tempVec = searchResult.empty() ? todos : searchResult;

	cout << setw(5) << left << "No." << " "
		 << setw(5) << left << "Importance" << " "
		 << setw(15) << left << "Category" << " "
		 << setw(15) << left << "Date" << " "
		 << setw(25) << left << "Task" << " "
		 << setw(12) << "Start time" << " " << setw(12) << "End time" << " "
		 << setw(15) << "Complete or not" << endl;


	for (i = 0; i < tempVec.size(); i++)
	{
		cout << setw(5) << left << i + 1 << " "
			 << setw(5) << left << tempVec[i].getImportance() << " "
			 << setw(15) << tempVec[i].getCategory() << " "
			 << setw(15) << left << tempVec[i].getDate() << " "
			 << setw(24) << tempVec[i].getTask() << " "
			 << setw(10) << tempVec[i].getStartTime() << " " << setw(12) << tempVec[i].getEndTime() << " "
			 << setw(12) << left << tempVec[i].getCheck() << endl;
	}
	cout << "\n";

	cout << "Enter a number of to-do you want to edit: ";
	cin >> userInput;

	return tempVec[i - 1];
}
