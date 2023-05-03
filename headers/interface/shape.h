#ifndef SHAPE_H
#define SHAPE_H

#include "SDL2/SDL.h"

#include "main/common.h"

class Rect;
class Circle;

class Shape
{
public:
	virtual bool in(const Position& pt) const = 0;
	virtual bool in(const Rect& rect) const = 0;
	//virtual bool in(const Circle& circle) const = 0;
	virtual ~Shape() {}
};

class Rect : public Shape
{
private:
	Position _topLeft;
	Position _bottomRight;

public:
	Rect(const Position& tl = Position(), const Position& br = Position());
	Rect(int x, int y, int w, int h) : _topLeft(Position(x,y)), _bottomRight(Position(x+w, y+h)) {}

	bool in(const Position& pt) const;

	const Position& getTopLeftCoor() const {return _topLeft;}
	const Position& getBottomRightCoor() const {return _bottomRight;}

	bool in(const Rect& rect) const;
	//bool in(const Circle& circle) const;

	SDL_Rect to_SDL_Rect() const;

	virtual ~Rect() {}
};

class Circle : public Shape
{
private:
	Position _center;
	double _radius;

public:
	bool in(const Position& pt) const;
	bool in(const Rect& rect) const {return false;}

	virtual ~Circle() {}
};

#endif //SHAPE_H