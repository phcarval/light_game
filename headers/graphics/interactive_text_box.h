#ifndef INTERACTIVE_TEXT_BOX_H
#define INTERACTIVE_TEXT_BOX_H

#include "SDL2/SDL.h"

#include "graphics/text_box.h"

class InteractiveTextBox : public TextBox
{
public:
	InteractiveTextBox(Window& window, const Rect& rect) : TextBox(window, "", rect) {SDL_StartTextInput();}
	~InteractiveTextBox() {SDL_StopTextInput();}

private:
	void manageKeyboard(const InputStatus& status) override;
};

#endif //INTERACTIVE_TEXT_BOX_H