#define _CRT_SECURE_NO_WARNINGS

#include "ToDoManagement.h"

using namespace std;
namespace fs = std::filesystem;

/* to-do에 입력한 날짜 포맷의 유효성 확인 [YYYY-MM-DD] */
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

	cout << "--------------[Enter the information about your To-do]---------------" << endl;

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
		cerr << "\nThere is no to-do list in ";	// 이 클래스에 날짜 정보를 저장할 수 없어 계속해서 리턴 코드 반환
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
			// 한 줄의 각 필드 parsing
			getline(ss, task, ',');
			getline(ss, date, ',');
			getline(ss, start_time, ',');
			getline(ss, end_time, ',');
			getline(ss, category, ',');

			// 중요도는 정수라 별도 token으로 관리
			getline(ss, token, ',');
			if (!token.empty()) {
				importance = stoi(token); // 이 과정에서 예외 발생 가능성 있음
			}

			// 완료 여부 역시 위와 유사
			getline(ss, token, ',');
			if (!token.empty()) {
				check = (token == "Y");
			}

			// ToDo 객체를 만들어 todos 벡터에 삽입
			// emplace_back을 사용해 매개변수만을 전달했음에도 자동으로 객체를 생성해줌
			todos.emplace_back(task, date, start_time, end_time, category, importance, check);

		}
		catch (const exception& e) {
			// 혹시모를 예외 발생 가능성 대비
			cerr << "Error parsing line: " << line << ". Error: " << e.what() << endl;
			continue;
		}
	}

	file.close();
	return 0;	// 정상 종료
}


/* 사용자가 "Load by date" 선택 시 결과 출력 */
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

/* 사용자가 "Load by category" 선택 시 결과 출력 */
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

/* 사용자가 "Load after sorting by importance" 선택 시 결과 출력 */
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
			 << setw(10) << left << todo.getEndTime()
			 << setw(9) << left << todo.getCheck() << endl;
	}
	cout << "\n";
}

/* 사용자가 원하는 to-do의 완료 여부를 변경하게 만드는 함수 */
ToDo ToDoManagement::printToDos_editMode()
{
	int userInput, i;
	vector<ToDo> tempVec = searchResult.empty() ? todos : searchResult;	// 둘 중 비어있지 않은 벡터가 현재 사용자가 입력한 정보가 담긴 벡터

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

	// 사용자로부터 완료 여부를 변경하고자 하는 ToDo의 번호를 입력받아 해당 ToDo 객체를 리턴
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

/* 매개변수로 주어진 to-do를 list 내에서 찾아 완료 여부를 변경하는 함수 */
void ToDoManagement::changeComplete(ToDo& todo)
{
	for (auto it = todos.begin(); it != todos.end(); it++) {
		if (it->getTask() == todo.getTask()) {
			it->taskComplete();
			return;
		}
	}
}
