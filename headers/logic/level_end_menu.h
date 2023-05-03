#ifndef LEVEL_END_MENU_H
#define LEVEL_END_MENU_H

#include "logic/main_levels.h"
#include "main/game_menu.h"

class LevelEndMenu : public Menu
{
public:
	enum {NEXT, LVLSELECT, MENU, QUIT};

	LevelNb _lvlNumber;

public:
	LevelEndMenu(Window& window, LevelNb number) : Menu(window), _lvlNumber(number) {init();}

	WindowContext* updateContext(const InputStatus& status) override;

private:
	void init();
};

#endif //LEVEL_END_MENU_H