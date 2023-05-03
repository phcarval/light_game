#ifndef GRAPHICAL_H
#define GRAPHICAL_H

#include <string>

#include "graphics/texture.h"
#include "interface/shape.h"

class Tile;

class Graphical
{
private:
	Texture* _texture;

protected:
	Rect _rect;

public:
	Graphical(Window& window, const std::string& path, const Rect& pos) :
									 _texture(new Texture(window, path)),
									 _rect(pos) {}

	Graphical(Window& window, const Rect& pos) : _texture(new Texture(window)), _rect(pos) {}

	Graphical(const Graphical& other) : _texture(new Texture(*(other._texture))), _rect(other._rect) {}
	Graphical(Graphical&& other) : _texture(other._texture), _rect(other._rect) {other._texture = NULL;}
	
	const Texture& getTexture() const {return *_texture;} // may NOT return NULL
	Texture& getTexture() {return *_texture;}

	virtual ~Graphical() {if(_texture != NULL) delete _texture;}

	Graphical& operator=(const Graphical& other);
	Graphical& operator=(Graphical&& other);

	const Rect& getRect() const {return _rect;}
	Rect& getRect() {return _rect;}

	virtual void render() const;

	bool in(const Position& pt) {return _rect.in(pt);}

	void updateText(const std::string& text) {if(_texture!=NULL) _texture->updateText(text);}
	void updateCounter(const std::string& counter) {if(_texture!=NULL) _texture->updateCounter(counter);}
	void updateTexture(const std::string& path) {if(_texture!=NULL) _texture->updateTexture(path);}

	void setColor(uint8_t red, uint8_t green, uint8_t blue) const;
};

#endif //GRAPHICAL_H