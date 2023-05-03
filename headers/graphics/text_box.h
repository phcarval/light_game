#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "interface/interactive.h"

class TextBox : public Interactive
{
protected:
	std::string _text;

public:
	TextBox(Window& window, const std::string& text, const Rect& rect) : Interactive(window, rect, false), _text(text) {updateText(text);}
	std::string getText() const {return _text;}
	void setText(const std::string nText) {_text = nText; updateText(nText);}
};

#endif //TEXT_BOX_H