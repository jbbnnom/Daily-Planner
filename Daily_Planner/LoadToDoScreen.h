#ifndef _LOAD_TO_DO_SCREEN_H_
#define _LOAD_TO_DO_SCREEN_H_

#include "ScreenManager.h"

class LoadToDoScreen : public ScreenManager {
public:
	LoadToDoScreen() = default;
	LoadToDoScreen(Planner& planner, ToDoManagement& tdm) : ScreenManager(planner, tdm) {};

	void drawScreen(int& mode) override;
	void showByDate();
	void showByCategory();
	void showByImportance();
};

#endif