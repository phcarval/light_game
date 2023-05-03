#ifndef BUTTON_H
#define BUTTON_H

#include "interface/interactive.h"

class Level;

class Button : public Interactive
{
public:
	Button(Window& window, const Rect& rect, bool textured = true)
									: Interactive(window,
									"/home/philippe/Documents/jeu de la mort/assets/button.png",
									rect,
									false) {if(!textured) this->updateTexture("");}
};

#endif //BUTTON_H