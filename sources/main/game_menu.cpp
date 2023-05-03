#include "main/game_menu.h"

void GameMenu::render()
{
	for(auto b : _buttons)
		b.second.render();
}
