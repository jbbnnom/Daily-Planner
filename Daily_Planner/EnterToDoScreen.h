#ifndef _ENTER_TO_DO_SCREEN_H_
#define _ENTER_TO_DO_SCREEN_H_

#include "ScreenManager.h"

/* to-do �Է� ȭ�� ��� Ŭ����
 * ScreenManager�� �ڽ� Ŭ�����̴�.
 */
class EnterToDoScreen : public ScreenManager {
public:
	EnterToDoScreen() = default;
	EnterToDoScreen(Planner& planner, ToDoManagement& tdm) : ScreenManager(planner, tdm) {};

	void drawScreen(int& mode) override;
};

#endif