#include <numeric>
#include <string>

#include "logic/level_select.h"
#include "main/main_menu.h"
#include "file_io/level_load.h"
#include "logic/main_levels.h"

WindowContext* LevelSelect::updateContext(const InputStatus& status)
{
	WindowContext* newContext = this;
	BUT_EFFECT effect = updateButtonEvents(status);
	//std::string lvlToLoad = "";
	switch(effect)
	{
		case BACK:
			newContext = new MainMenu(_window);
			break;

		case NONE:
			break;

		default:
			newContext = new Level(_window, MainLevels::getLevel(effect), effect);
			break;
	}

	/*if(!lvlToLoad.empty())
		newContext = LevelLoader(lvlToLoad, _window).load();*/

	return newContext;
}

void LevelSelect::init()
{
	std::vector<BUT_EFFECT> buttons((size_t) (MainLevels::getUnlockedLevels()+1));
	std::iota(buttons.begin(), buttons.end(), 0); // 0 = BACK, 1 = level 1...
	std::vector<std::string> texts({"back to menu"});

	for(int i = 1 ; i <= buttons.size() ; i++)
		texts.push_back("level " + std::to_string(i));

	defaultButtonInit(buttons,texts);
}
