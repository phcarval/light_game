#include "logic/options_menu.h"
#include "main/main_menu.h"
#include "logic/main_levels.h"

void OptionsMenu::init()
{
	defaultButtonInit({BACK, RESET},{"back to main menu","reset game progress"});
}

WindowContext* OptionsMenu::updateContext(const InputStatus& status)
{
	WindowContext* newContext = this;

	BUT_EFFECT effect = updateButtonEvents(status);
	if(effect == BACK)
		newContext = new MainMenu(_window);
	else if(effect == RESET)
		MainLevels::resetUnlockedLevels();
	else; // effect = NONE.

	return newContext;
}
