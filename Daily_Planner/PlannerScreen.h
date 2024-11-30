#ifndef _PLANNER_SCREEN_H_
#define _PLANNER_SCREEN_H_

#include "ScreenManager.h"


class PlannerScreen : public ScreenManager {
private:
	Planner planner;
public:
	PlannerScreen() = default;
	PlannerScreen(const Planner &pl) : planner(pl) {};

	void drawScreen(int& mode) override;
};

#endif