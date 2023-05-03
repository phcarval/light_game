#include <iostream>

#include "graphics/graphical.h"

Graphical& Graphical::operator=(const Graphical& other)
{
	delete this->_texture;
	this->_texture = NULL;
	this->_texture = new Texture(*(other._texture));

	this->_rect = other._rect;
	return *this;
}

Graphical& Graphical::operator=(Graphical&& other)
{
	delete this->_texture;
	this->_texture = NULL;
	this->_texture = other._texture;
	other._texture = NULL;

	this->_rect = other._rect;
	return *this;
}

void Graphical::render() const
{
	this->_texture->render(this->getRect());
}

void Graphical::setColor(uint8_t red, uint8_t green, uint8_t blue) const
{
	_texture->setColor(red,green,blue);
}
