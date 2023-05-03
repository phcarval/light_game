#include "command_input/input_status.h"
#include "main/main_menu.h"
#include "interface/text_button.h"
#include "file_io/level_load.h"
#include "logic/editor_size_selection.h"
#include "logic/level_select.h"
#include "logic/custom_level_menu.h"
#include "logic/options_menu.h"

WindowContext* MainMenu::updateContext(const InputStatus& status)
{
	WindowContext* newContext = this;

	BUT_EFFECT effect = updateButtonEvents(status);
	if(effect == START_GAME)
	{
		//newContext = LevelLoader("/home/philippe/Documents/jeu de la mort/levels/level_1.lvl", _window).load();
		newContext = new LevelSelect(_window);
	}
	else if(effect == EDITOR)
	{
		newContext = new EditorSizeSelection(_window);
	}
	else if(effect == CUSTOM_LEVELS)
	{
		newContext = new CustomLevelMenu(_window);
	}
	else if(effect == OPTIONS)
	{
		newContext = new OptionsMenu(_window);
	}
	else if(effect == QUIT)
	{
		newContext = NULL;
	}
	else; //effect == NONE
	return newContext;
}

void MainMenu::init()
{
	defaultButtonInit({START_GAME,EDITOR,CUSTOM_LEVELS,OPTIONS,QUIT},{"start game","start editor","load custom level","options","quit game"});
	/*int x = _window.getWidth()/3, y = _window.getHeight()/4, w = x, h = _window.getHeight()/6;
	_buttons.insert({START_GAME,TextButton(_window,{x,y,w,h},"start game")});

	x = _window.getWidth()/3, y = 2*_window.getHeight()/4, w = x, h = _window.getHeight()/6;
	_buttons.insert({EDITOR,TextButton(_window,{x,y,w,h},"start editor")});

	x = _window.getWidth()/3, y = 3*_window.getHeight()/4, w = x, h = _window.getHeight()/6;
	_buttons.insert({QUIT,TextButton(_window,{x,y,w,h},"quit game")});*/
}
