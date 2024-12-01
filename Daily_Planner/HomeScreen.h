#ifndef _HOME_SCREEN_H_
#define _HOME_SCREEN_H_

#include "ScreenManager.h"

class HomeScreen : public ScreenManager {
public:
	HomeScreen() = default;
	HomeScreen(Planner& planner, ToDoManagement& tdm) : ScreenManager(planner, tdm) {};

	void drawScreen(int& mode) override;
};

#endif