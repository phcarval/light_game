#ifndef TEXT_BUTTON_H
#define TEXT_BUTTON_H

#include <string>

#include "main/common.h"
#include "interface/button.h"

class TextButton : public Button
{

public:
	TextButton(Window& window,
			   const Rect& rect,
			   const std::string& text) : Button(window, rect, false) {this->updateText(text);}
};

#endif //TEXT_BUTTON_H