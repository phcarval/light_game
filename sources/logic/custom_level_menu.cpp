#include <numeric>
#include <dirent.h>

#include "logic/custom_level_menu.h"
#include "logic/level.h"
#include "main/main_menu.h"

std::vector<Menu::BUT_EFFECT> CustomLevelMenu::createEffectVector()
{
	std::vector<BUT_EFFECT> effects(_levels.size()); // BUT_EFFECT is int
	std::iota(effects.begin(), effects.end(), 0/*= QUIT*/); // effects = [0, 1, 2, 3, 4, ...]
	return effects;
}

std::string CustomLevelMenu::manageEffect(BUT_EFFECT effect)
{
	std::string lvl = "";
	if(effect < _levels.size())
	{
		lvl = _levels[effect];
	}
	else
	{
		std::cout << "error : level chosen is impossible" << std::endl;
	}
	return lvl;
}

void CustomLevelMenu::init()
{
	_levels.push_back("return to menu");
	std::string customLvDir = "/home/philippe/Documents/jeu de la mort/custom_levels/";

	DIR* lvDir = nullptr;
	struct dirent* file = nullptr;

	lvDir = opendir(customLvDir.c_str());
	if(lvDir == NULL) // or nullptr? opendir is a C function so *should* return NULL
	{
		std::cout << "Bad directory for loading custom levels" << std::endl;
	}
	else
	{
		while((file = readdir(lvDir)))
		{
			std::string curName = file->d_name;
			if(curName != "." && curName != "..")
				_levels.push_back(customLvDir + curName);
		}
		closedir(lvDir);
	}

	this->defaultButtonInit(createEffectVector(), _levels);
}

WindowContext* CustomLevelMenu::updateContext(const InputStatus& status)
{
	WindowContext* newContext = this;

	BUT_EFFECT effect = updateButtonEvents(status);
	if(effect == QUIT)
	{
		newContext = new MainMenu(_window);
	}
	else if(effect != NONE)
	{
		std::string lvl = manageEffect(effect);
		try
		{
			newContext = new Level(_window, lvl);
		}
		catch(const LevelException& e)
		{
			std::cout << e.what() << std::endl;
			newContext = this;
		}
	}
	return newContext;
}
