#include "main/window.h"
#include "interface/text_button.h"
#include "logic/pause_menu.h"
#include "main/main_menu.h"
#include "logic/level.h"

void PauseMenu::init()
{
	defaultButtonInit({CONTINUE,STOP,QUIT},{"continue game","return to main menu","quit game"});
}

WindowContext* PauseMenu::updateContext(const InputStatus& status)
{
	WindowContext* newContext = this;
	BUT_EFFECT effect = updateButtonEvents(status);
	if(effect == CONTINUE)
	{
		newContext = &_level;
	}
	else if(effect == STOP)
	{
		newContext = new MainMenu(_window);
	}
	else if(effect == QUIT)
		newContext = nullptr;
	else; //effect = NONE

	return newContext;
}
