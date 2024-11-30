#include "Planner.h"

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

// 콘솔에 달력 출력
void Planner::printCalendar()
{
	unsigned startPos, dayPos;

	// 입력한 년도와 달의 1일과 말일 정보를 저장
	year_month_day firstDay{ ym->year() / ym->month() / 1d };
	weekday firstWeekDay = weekday{ sys_days{firstDay} };
	year_month_day_last lastDay{ *ym / last };
	
	cout.width(20);
	cout << stringMonth[static_cast<unsigned>(ym->month())] << " " << ym->year() << "\n\n";

	cout.width(5);
	cout << std::right << "SUN";
	cout.width(5);
	cout << std::right << "MON";
	cout.width(5);
	cout << std::right << "TUE";
	cout.width(5);
	cout << std::right << "WED";
	cout.width(5);
	cout << std::right << "THU";
	cout.width(5);
	cout << std::right << "FRI";
	cout.width(5);
	cout << std::right << "SAT";
	cout << endl;

	// 해당 월의 시작 요일 확인 후 시작일 간격 조정 [c_encoding()의 범위: 0(SUN)~6(SAT)]
	startPos = (firstWeekDay.c_encoding() + 1) * 5;
	cout.width(startPos);
	
	// 달력 출력
	dayPos = firstWeekDay.c_encoding();
	for (unsigned i = 1; i <= unsigned(lastDay.day()); i++) {
		cout << std::right << i;
		if (dayPos < 6) {
			dayPos++;
		}
		else {
			dayPos = 0;
			cout << endl;
		}
		cout.width(5);
	}
}

// 오늘의 할 일을 텍스트 파일에 저장
void Planner::writeToFile()
{
	string task = "Complete OOP proj4";

	setPlannerPath();
	fstream file(plannerPath, ios::app);

	if (file.is_open()) {
		file << task << endl;
	}
}

// 메인화면에 오늘의 할일을 보여주기 위해 오늘 일자의 텍스트 파일로부터 할 일을 가져오는 함수
void Planner::readToDo()
{

}

// To-do가 적힌 텍스트 파일이 저장되는 경로 지정
// 바탕화면에 있는 플래너 폴더에 '년-월-일' 순서대로 폴더가 생성되어 입력한 일자에 저장된다
void Planner::setPlannerPath()
{
	string yearStr = to_string(static_cast<int>(ym->year()));
	plannerPath /= yearStr;

	plannerPath /= stringMonth[static_cast<unsigned>(ym->month())];

	string dayStr = format("{}", *day);
	plannerPath /= dayStr;

	if (!fs::exists(plannerPath)) {
		fs::create_directories(plannerPath);
	}

	plannerPath /= "To-do list.txt";
}