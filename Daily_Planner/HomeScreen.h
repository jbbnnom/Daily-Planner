#ifndef _HOME_SCREEN_H_
#define _HOME_SCREEN_H_

#include "ScreenManager.h"

/* 시작 화면 담당 클래스
 * ScreenManager의 자식 클래스이다.
 */
class HomeScreen : public ScreenManager {
public:
	HomeScreen() = default;
	HomeScreen(Planner& planner, ToDoManagement& tdm) : ScreenManager(planner, tdm) {};

	void drawScreen(int& mode) override;
};

#endif