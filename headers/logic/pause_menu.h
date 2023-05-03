#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <map>

#include "main/game_menu.h"

class Button;

class PauseMenu : public GameMenu
{
public:
	enum {CONTINUE, STOP, QUIT};

public:
	PauseMenu(Window& window, Level& level) : GameMenu(window, level) {init();}

	WindowContext* updateContext(const InputStatus& status) override;

private:
	void init();
};

#endif //PAUSE_MENU_H