#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "main/menu.h"

class GameMenu : public Menu
{
protected:
	Level& _level;

public:
	GameMenu(Window& window, Level& level) : Menu(window), _level(level) {}
	virtual ~GameMenu() {}

	void render() override;
};

#endif //GAME_MENU_H