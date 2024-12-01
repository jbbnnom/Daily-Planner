#ifndef _ENTER_TO_DO_SCREEN_H_
#define _ENTER_TO_DO_SCREEN_H_

#include "ScreenManager.h"

class EnterToDoScreen : public ScreenManager {
private:
	ToDo* todo;
public:
	EnterToDoScreen() = default;
	EnterToDoScreen(Planner& planner, ToDoManagement& tdm) : ScreenManager(planner, tdm) {};

	void drawScreen(int& mode) override;
	void makeNewToDo();
};

#endif