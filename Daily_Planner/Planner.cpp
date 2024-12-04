#include "Planner.h"

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

/* 콘솔에 달력 출력 */
void Planner::printCalendar()
{
	unsigned startPos, dayPos;

	// 입력한 년도와 달의 1일과 말일 정보를 저장
	year_month_day firstDay{ ym->year() / ym->month() / 1d };
	weekday firstWeekDay = weekday{ sys_days{firstDay} };
	year_month_day_last lastDay{ *ym / last };

	cout.flush();

	cout << setw(35) << stringMonth[static_cast<unsigned>(ym->month())] << " " << ym->year() << "\n\n";

	cout << setw(20) << std::right << "SUN";
	cout << setw(5) << std::right << "MON";
	cout << setw(5) << std::right << "TUE";
	cout << setw(5) << std::right << "WED";
	cout << setw(5) << std::right << "THU";
	cout << setw(5) << std::right << "FRI";
	cout << setw(5) << std::right << "SAT";
	cout << endl;

	// 해당 월의 시작 요일 확인 후 시작일 간격 조정 [c_encoding()의 범위: 0(SUN)~6(SAT)]
	startPos = (firstWeekDay.c_encoding() + 1) * 5 + 15;
	cout << setw(startPos);
	
	// 달력 출력
	dayPos = firstWeekDay.c_encoding();
	for (unsigned i = 1; i <= unsigned(lastDay.day()); i++) {
		cout << std::right << i;
		if (dayPos < 6) {
			dayPos++;
			cout << setw(5);
		}
		else {
			dayPos = 0;
			cout << endl;
			cout << setw(20);
		}
	}
	cout << endl;
}


/* 사용자가 입력한 할 일 하나를 텍스트 파일에 저장 */
void Planner::writeToFile(ToDo &todo)
{
	setPlannerPath(1);
	fstream file(plannerPath, ios::app);	// 파일이 이미 있는 경우 이어서 작성해야 함

	if (file.is_open()) {
		ToDoManagement::saveToDo(file, todo);
	}

	file.close();
}


/* 모든 일자의 텍스트 파일로부터 할 일을 가져와 todos 벡터에 저장 
 * (카테고리별 검색 및 중요도 순 정렬에 활용)
 */
void Planner::loadAllToDos(ToDoManagement& tdm)
{
	// 시작 전 todos 벡터를 비우고 경로를 "~\Desktop\Daily Planner"로 변경
	tdm.clearToDos();
	resetPlannerPath();

	// 경로의 유효성 검사
	if (!fs::exists(plannerPath) || !fs::is_directory(plannerPath)) {
		cout << "Invalid path: " << plannerPath << endl;
		return;
	}

	// Daily Planner의 모든 하위 디렉터리, 즉 모든 일자의 to-do list를 검색해 todos 벡터에 저장
	for (const auto& entry : fs::recursive_directory_iterator(plannerPath)) {
		if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".txt") {
			tdm.loadOneDayToDos(entry.path());
		}
	}
}

/* 내용이 변경된 to-do를 파일에 덮어씌우는 함수 */
void Planner::overlapToDos(ToDoManagement &tdm)
{
	fstream file(plannerPath, ios::out);
	vector<ToDo> changedToDos = tdm.getToDos();

	for (auto it = changedToDos.begin(); it != changedToDos.end(); it++) {
		tdm.saveToDo(file, *it);
	}
}


/* To-do가 적힌 텍스트 파일이 저장되는 경로 지정
 * 바탕화면에 있는 플래너 폴더에 '년-월-일' 순서대로 폴더가 생성되어 입력한 일자에 저장된다
 */
void Planner::setPlannerPath(int mode)
{
	string yearStr, dayStr;

	// 항상 안전하게 기 저장된 path를 지우고 시작
	resetPlannerPath();

	if (mode == 0) {	// year_month_day 객체로부터 설정
		yearStr = to_string(static_cast<int>(ymd->year()));
		plannerPath /= yearStr;

		plannerPath /= stringMonth[static_cast<unsigned>(ymd->month())];

		dayStr = format("{}", ymd->day());
		plannerPath /= dayStr;
	}
	else if (mode == 1) {	// year_month 객체와 day 객체로부터 설정
		yearStr = to_string(static_cast<int>(ym->year()));
		plannerPath /= yearStr;
		
		plannerPath /= stringMonth[static_cast<unsigned>(ym->month())];

		dayStr = format("{}", *d);
		plannerPath /= dayStr;
	}

	if (!fs::exists(plannerPath)) {
		fs::create_directories(plannerPath);
	}

	plannerPath /= "To-do list.txt";
}


/* year_month 객체에 년/월 정보 저장
 * (EnterToDoScreen에서 년과 월만 받고 달력을 출력하기 때문에 별도로 생성함)
 */
void Planner::setYearMonth(int yearValue, int monthValue)
{
	if (ym != nullptr) {
		delete ym;
	}
	ym = new year_month{ year(yearValue) / month(monthValue) };
}

/* day 객체에 일 정보 저장 */
void Planner::setDay(int dayValue)
{
	if (d != nullptr) {
		delete d;
	}
	d = new day(dayValue);
}

/* year_month_day 객체에 년/월/일 정보 저장 [Overloading #1 - year_month 타입 멤버와 day 타입 멤버로부터] */
void Planner::setYearMonthDay()
{
	if (ymd != nullptr) {
		delete ymd;
	}
	ymd = new year_month_day(ym->year() / ym->month() / *d);
}

/* year_month_day 객체에 년/월/일 정보 저장 [Overloading #2 - 매개변수로 받은 년/월/일의 정수값으로부터] */
void Planner::setYearMonthDay(int yearValue, int monthValue, int dayValue)
{
	if (ymd != nullptr) {
		delete ymd;
	}
	ymd = new year_month_day{ year(yearValue) / month(monthValue) / day(dayValue)};
}

/* year_month_day 객체에 년/월/일 정보 저장 [Overloading #3 - 매개변수로 받은 year_month_day 객체로부터] */
void Planner::setYearMonthDay(year_month_day& p_ymd)
{
	if (ymd != nullptr) {
		delete ymd;
	}
	ymd = new year_month_day{ p_ymd.year() / p_ymd.month() / p_ymd.day() };
}


/* year_month_day 객체를 문자열(ex. December 9, 2024)로 변환 */
string dateToStr(year_month_day& ymd)
{
	vector<const char*> stringMonth = { "",
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"
	};

	return string(stringMonth[static_cast<unsigned>(ymd.month())]) +
		" " + to_string(static_cast<unsigned>(ymd.day())) +
		", " + to_string(static_cast<int>(ymd.year()));
}