#ifndef CUSTOM_LEVEL_MENU_H
#define CUSTOM_LEVEL_MENU_H

#include "main/menu.h"

class CustomLevelMenu : public Menu
{
private:
	std::vector<std::string> _levels;

	enum {QUIT = 0};

public:
	CustomLevelMenu(Window& window) : Menu(window) {init();}

	WindowContext* updateContext(const InputStatus& status) override;

private:
	void init();

	inline std::vector<BUT_EFFECT> createEffectVector();
	inline std::string manageEffect(BUT_EFFECT effect);
};

#endif //CUSTOM_LEVEL_MENU_H