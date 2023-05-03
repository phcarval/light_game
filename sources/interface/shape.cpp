#include <math.h>
#include <exception>

#include "main/common.h"
#include "interface/shape.h"

Rect::Rect(const Position& tl, const Position& br) : _topLeft(tl), _bottomRight(br)
{
	if(tl.x > br.x || tl.y > br.y)
		throw std::invalid_argument("Rect initialization failed : invalid coordinates");
}

bool Rect::in(const Position& pt) const
{
	bool isIn = false;
	if(pt.x > _topLeft.x &&
	   pt.x < _bottomRight.x &&
	   pt.y > _topLeft.y &&
	   pt.y < _bottomRight.y)
	{
		isIn = true;
	}
	return isIn;
}

bool Rect::in(const Rect& rect) const
{
	bool isIn = false;
	Position topLeft = this->getTopLeftCoor();
	Position bottomRight = this->getBottomRightCoor();

	if(rect.getTopLeftCoor()>topLeft && rect.getBottomRightCoor()<bottomRight)
		isIn = true;

	return isIn;
}

bool Circle::in(const Position& pt) const
{
	bool isIn = false;
	unsigned int a = _center.x, b = pt.x, c = _center.y, d = pt.y;
	
	float distance = sqrt((a-b)^2 + (c-d)^2);

	if(distance < _radius)
		isIn = true;
	return isIn;
}

SDL_Rect Rect::to_SDL_Rect() const
{
	int x = this->_topLeft.x;
	int y = this->_topLeft.y;

	int w = (this->_bottomRight.x-this->_topLeft.x);
	int h = (this->_bottomRight.y-this->_topLeft.y);

	return {x,y,w,h};
}
