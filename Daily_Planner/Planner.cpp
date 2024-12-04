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

	// �ش� ���� ���� ���� Ȯ�� �� ������ ���� ���� [c_encoding()�� ����: 0(SUN)~6(SAT)]
	startPos = (firstWeekDay.c_encoding() + 1) * 5 + 15;
	cout << setw(startPos);
	
	// �޷� ���
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


/* ����ڰ� �Է��� �� �� �ϳ��� �ؽ�Ʈ ���Ͽ� ���� */
void Planner::writeToFile(ToDo &todo)
{
	setPlannerPath(1);
	fstream file(plannerPath, ios::app);	// ������ �̹� �ִ� ��� �̾ �ۼ��ؾ� ��

	if (file.is_open()) {
		ToDoManagement::saveToDo(file, todo);
	}

	file.close();
}


/* ��� ������ �ؽ�Ʈ ���Ϸκ��� �� ���� ������ todos ���Ϳ� ���� 
 * (ī�װ��� �˻� �� �߿䵵 �� ���Ŀ� Ȱ��)
 */
void Planner::loadAllToDos(ToDoManagement& tdm)
{
	// ���� �� todos ���͸� ���� ��θ� "~\Desktop\Daily Planner"�� ����
	tdm.clearToDos();
	resetPlannerPath();

	// ����� ��ȿ�� �˻�
	if (!fs::exists(plannerPath) || !fs::is_directory(plannerPath)) {
		cout << "Invalid path: " << plannerPath << endl;
		return;
	}

	// Daily Planner�� ��� ���� ���͸�, �� ��� ������ to-do list�� �˻��� todos ���Ϳ� ����
	for (const auto& entry : fs::recursive_directory_iterator(plannerPath)) {
		if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".txt") {
			tdm.loadOneDayToDos(entry.path());
		}
	}
}

/* ������ ����� to-do�� ���Ͽ� ������ �Լ� */
void Planner::overlapToDos(ToDoManagement &tdm)
{
	fstream file(plannerPath, ios::out);
	vector<ToDo> changedToDos = tdm.getToDos();

	for (auto it = changedToDos.begin(); it != changedToDos.end(); it++) {
		tdm.saveToDo(file, *it);
	}
}


/* To-do�� ���� �ؽ�Ʈ ������ ����Ǵ� ��� ����
 * ����ȭ�鿡 �ִ� �÷��� ������ '��-��-��' ������� ������ �����Ǿ� �Է��� ���ڿ� ����ȴ�
 */
void Planner::setPlannerPath(int mode)
{
	string yearStr, dayStr;

	// �׻� �����ϰ� �� ����� path�� ����� ����
	resetPlannerPath();

	if (mode == 0) {	// year_month_day ��ü�κ��� ����
		yearStr = to_string(static_cast<int>(ymd->year()));
		plannerPath /= yearStr;

		plannerPath /= stringMonth[static_cast<unsigned>(ymd->month())];

		dayStr = format("{}", ymd->day());
		plannerPath /= dayStr;
	}
	else if (mode == 1) {	// year_month ��ü�� day ��ü�κ��� ����
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


/* year_month ��ü�� ��/�� ���� ����
 * (EnterToDoScreen���� ��� ���� �ް� �޷��� ����ϱ� ������ ������ ������)
 */
void Planner::setYearMonth(int yearValue, int monthValue)
{
	if (ym != nullptr) {
		delete ym;
	}
	ym = new year_month{ year(yearValue) / month(monthValue) };
}

/* day ��ü�� �� ���� ���� */
void Planner::setDay(int dayValue)
{
	if (d != nullptr) {
		delete d;
	}
	d = new day(dayValue);
}

/* year_month_day ��ü�� ��/��/�� ���� ���� [Overloading #1 - year_month Ÿ�� ����� day Ÿ�� ����κ���] */
void Planner::setYearMonthDay()
{
	if (ymd != nullptr) {
		delete ymd;
	}
	ymd = new year_month_day(ym->year() / ym->month() / *d);
}

/* year_month_day ��ü�� ��/��/�� ���� ���� [Overloading #2 - �Ű������� ���� ��/��/���� ���������κ���] */
void Planner::setYearMonthDay(int yearValue, int monthValue, int dayValue)
{
	if (ymd != nullptr) {
		delete ymd;
	}
	ymd = new year_month_day{ year(yearValue) / month(monthValue) / day(dayValue)};
}

/* year_month_day ��ü�� ��/��/�� ���� ���� [Overloading #3 - �Ű������� ���� year_month_day ��ü�κ���] */
void Planner::setYearMonthDay(year_month_day& p_ymd)
{
	if (ymd != nullptr) {
		delete ymd;
	}
	ymd = new year_month_day{ p_ymd.year() / p_ymd.month() / p_ymd.day() };
}


/* year_month_day ��ü�� ���ڿ�(ex. December 9, 2024)�� ��ȯ */
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