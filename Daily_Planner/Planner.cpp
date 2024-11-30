#include "Planner.h"

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

// �ֿܼ� �޷� ���
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

// ������ �� ���� �ؽ�Ʈ ���Ͽ� ����
void Planner::writeToFile()
{
	string task = "Complete OOP proj4";

	setPlannerPath();
	fstream file(plannerPath, ios::app);

	if (file.is_open()) {
		file << task << endl;
	}
}

// ����ȭ�鿡 ������ ������ �����ֱ� ���� ���� ������ �ؽ�Ʈ ���Ϸκ��� �� ���� �������� �Լ�
void Planner::readToDo()
{

}

// To-do�� ���� �ؽ�Ʈ ������ ����Ǵ� ��� ����
// ����ȭ�鿡 �ִ� �÷��� ������ '��-��-��' ������� ������ �����Ǿ� �Է��� ���ڿ� ����ȴ�
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