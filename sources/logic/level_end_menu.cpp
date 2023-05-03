#include "logic/level_end_menu.h"
#include "main/main_menu.h"
#include "logic/level_select.h"
#include "logic/level.h"

void LevelEndMenu::init()
{
	if(_lvlNumber < MainLevels::getNumberOfLevels() && _lvlNumber >= 1)
		defaultButtonInit({NEXT,LVLSELECT,MENU,QUIT},{"next level","level select menu","main menu","quit game"});
	else
		defaultButtonInit({LVLSELECT,MENU,QUIT},{"level select menu","main menu","quit game"});

}

WindowContext* LevelEndMenu::updateContext(const InputStatus& status)
{
	WindowContext* newContext = this;
	BUT_EFFECT effect = updateButtonEvents(status);
	
	switch(effect)
	{
		case MENU:
			newContext = new MainMenu(_window);
			break;

		case LVLSELECT:
			newContext = new LevelSelect(_window);
			break;

		case QUIT:
			newContext = nullptr;
			break;

		case NEXT:
			_lvlNumber++;
			try
			{
				newContext = new Level(_window,MainLevels::getLevel(_lvlNumber),_lvlNumber);
			}
			catch(const LevelException& e)
			{
				std::cout << "Level " << _lvlNumber << " not found : " << e.what() << std::endl;
				newContext = this;
			}
			break;

		default:
			//effect == NONE;
			break;
	}

	return newContext;
}
