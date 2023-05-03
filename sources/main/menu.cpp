#include "main/window.h"
#include "main/menu.h"
#include "interface/text_button.h"

void Menu::render()
{
	_window.renderClear();
	for(auto b : _buttons)
		b.second.render();
	_window.renderDraw();
}

Menu::BUT_EFFECT Menu::updateButtonEvents(const InputStatus& status)
{
	BUT_EFFECT eff = NONE;
	for(auto b : _buttons)
	{
		b.second.updateEvents(status);
		if(b.second.isClicking(M_LEFT))
		{
			eff = b.first;
		}
	}
	return eff;
}

void Menu::defaultButtonInit(std::vector<BUT_EFFECT> effects, std::vector<std::string> captions)
{
	int n_effects = effects.size();
	for(int i = 0 ; i < n_effects ; i++)
	{
		int x = _window.getWidth()/3, y = i*_window.getHeight()/(n_effects+1), w = x, h = _window.getHeight()/(n_effects+2);
		_buttons.insert({effects[i],TextButton(_window,{x,y,w,h},captions[i])});
	}
}
