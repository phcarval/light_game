#ifndef LEVEL_SELECT_H
#define LEVEL_SELECT_H

#include "main/menu.h"

class LevelSelect : public Menu
{
private:
	enum {BACK = 0};

public:
	LevelSelect(Window& window) : Menu(window) {init();}
	WindowContext* updateContext(const InputStatus& status) override;

private:
	void init();
};

#endif //LEVEL_SELECT_H