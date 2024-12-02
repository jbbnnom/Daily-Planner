#ifndef _ENTER_TO_DO_SCREEN_H_
#define _ENTER_TO_DO_SCREEN_H_

#include "ScreenManager.h"

/* to-do 입력 화면 담당 클래스
 * ScreenManager의 자식 클래스이다.
 */
class EnterToDoScreen : public ScreenManager {
public:
	EnterToDoScreen() = default;
	EnterToDoScreen(Planner& planner, ToDoManagement& tdm) : ScreenManager(planner, tdm) {};

	void drawScreen(int& mode) override;
};

#endif