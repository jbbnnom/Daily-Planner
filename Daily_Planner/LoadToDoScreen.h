#ifndef _LOAD_TO_DO_SCREEN_H_
#define _LOAD_TO_DO_SCREEN_H_

#include "ScreenManager.h"

/* to-do �б� �� ���� ȭ�� ��� Ŭ����
 * ScreenManager�� �ڽ� Ŭ�����̴�.
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