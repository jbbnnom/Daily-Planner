#include "Planner.h"

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

/* �ֿܼ� �޷� ��� */
void Planner::printCalendar()
{
	unsigned startPos, dayPos;

	// �Է��� �⵵�� ���� 1�ϰ� ���� ������ ����
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

	// �ش� ���� ���� ���� Ȯ�� �� ������ ���� ���� [c_encoding()�� ����: 0(SUN)~6(SAT)]
	startPos = (firstWeekDay.c_encoding() + 1) * 5;
	cout.width(startPos);
	
	// �޷� ���
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


/* ����ڰ� �Է��� �� �� �ϳ��� �ؽ�Ʈ ���Ͽ� ���� */
void Planner::writeToFile(ToDo &todo)
{
	setPlannerPath(3);
	fstream file(plannerPath, ios::app);	// ������ �̹� �ִ� ��� �̾ �ۼ��ؾ� ��

	if (file.is_open()) {
		ToDoManagement::save_toFile(file, todo);
	}

	file.close();
}


/* ��� ������ �ؽ�Ʈ ���Ϸκ��� �� ���� �������� �Լ� */
void Planner::loadAllToDos(ToDoManagement& tdm)
{
	tdm.clearToDos();
	resetPlannerPath();

	if (!fs::exists(plannerPath) || !fs::is_directory(plannerPath)) {
		cout << "Invalid path: " << plannerPath << endl;
		return;
	}

	for (const auto& entry : fs::recursive_directory_iterator(plannerPath)) {
		if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".txt") {
			tdm.loadOneDayToDos(plannerPath);
			cout << "Successfully loaded : " << plannerPath << endl;	// test output
		}
	}
}



void Planner::setYearMonth(int yearValue, int monthValue)
{
	if (ym != nullptr) {
		delete ym;
	}
	ym = new year_month{ year(yearValue) / month(monthValue) };
}

void Planner::setDay(int dayValue)
{
	if (d != nullptr) {
		delete d;
	}
	d = new day(dayValue);
}

void Planner::setYearMonthDay()
{
	if (ymd != nullptr) {
		delete ymd;
	}
	ymd = new year_month_day(ym->year() / ym->month() / *d);
}


/* To-do�� ���� �ؽ�Ʈ ������ ����Ǵ� ��� ����
 * ����ȭ�鿡 �ִ� �÷��� ������ '��-��-��' ������� ������ �����Ǿ� �Է��� ���ڿ� ����ȴ�
 */
void Planner::setPlannerPath(int mode)
{
	resetPlannerPath();

	if (mode >= 1) {
		string yearStr = to_string(static_cast<int>(ym->year()));
		plannerPath /= yearStr;
	}
	if (mode >= 2) {
		plannerPath /= stringMonth[static_cast<unsigned>(ym->month())];
	}
	if (mode >= 3) {
		string dayStr = format("{}", *d);
		plannerPath /= dayStr;
	}

	if (!fs::exists(plannerPath)) {
		fs::create_directories(plannerPath);
	}

	plannerPath /= "To-do list.txt";
}