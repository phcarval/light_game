#ifndef TEXT_H
#define TEXT_H

#include <string>

#include "SDL2/SDL_ttf.h"

class Text
{
private:
	std::string _text;
	TTF_Font* _font;

public:
	Text(std::string text, TTF_Font* font = "Calibri.ttf"/*default*/) : _text(text), _font(font) {}

	const std::string& getText() const {return _text;}
	const TTF_Font* getFont() const {return _font;}

	void setText(const std::string& text) {_text = text;}
	void setFont(TTF_Font* font) {_font = font;}
};

#endif //TEXT_H