#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <map>

#include "main/menu.h"
#include "interface/text_button.h"

class InputStatus;

class MainMenu : public Menu
{
private:
	enum {START_GAME, EDITOR, CUSTOM_LEVELS, OPTIONS, QUIT};

public:
	MainMenu(Window& window) : Menu(window) {init();}

	WindowContext* updateContext(const InputStatus& status) override;

private:
	void init();
};

#endif //MAIN_MENU_H