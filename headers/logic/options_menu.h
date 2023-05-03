#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include "main/menu.h"

class OptionsMenu : public Menu
{
private:
	enum {BACK, RESET};

public:
	OptionsMenu(Window& window) : Menu(window) {init();}
	WindowContext* updateContext(const InputStatus& status) override;

private:
	void init();
};

#endif //OPTIONS_MENU_H