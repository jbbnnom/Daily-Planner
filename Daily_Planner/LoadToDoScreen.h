#ifndef _LOAD_TO_DO_SCREEN_H_
#define _LOAD_TO_DO_SCREEN_H_

#include "ScreenManager.h"

/* to-do 읽기 및 쓰기 화면 담당 클래스
 * ScreenManager의 자식 클래스이다.
 */
class LoadToDoScreen : public ScreenManager {
public:
	LoadToDoScreen() = default;
	LoadToDoScreen(Planner& planner, ToDoManagement& tdm) : ScreenManager(planner, tdm) {};

	void drawScreen(int& mode) override;

	int showByDate();
	int showByCategory();
	void showByImportance();
	void editToDo();
};

#endif