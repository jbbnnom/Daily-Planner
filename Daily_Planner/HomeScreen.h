#ifndef _HOME_SCREEN_H_
#define _HOME_SCREEN_H_

#include "ScreenManager.h"

class HomeScreen : public ScreenManager {
private:
	
public:
	HomeScreen() = default;
	void drawScreen(int& mode) override;
	void drawTitle(HANDLE handle);
};

#endif