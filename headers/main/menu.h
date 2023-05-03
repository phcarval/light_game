#ifndef MENU_H
#define MENU_H

#include <map>
#include <vector>

#include "main/window_context.h"
#include "interface/button.h"

class Menu : public WindowContext
{
public:
	typedef int BUT_EFFECT;
	enum {NONE = -1};

protected:
	std::map<BUT_EFFECT,Button> _buttons;
	Window& _window;

	BUT_EFFECT updateButtonEvents(const InputStatus& status);

public:
	Menu(Window& window) : _window(window) {}

	virtual void render() override;

protected:
	void defaultButtonInit(std::vector<BUT_EFFECT> effects, std::vector<std::string> captions);
};

#endif //MENU_H