#ifndef LIGHT_H
#define LIGHT_H

#include "logic/logic_common.h"
#include "interface/shape.h"
#include "graphics/graphical.h"

enum L_INTENSITY {WEAK, MEDIUM, HIGH};
enum L_COLOR {WHITE, BLUE, GREEN, RED};

struct LightShape
{
	DIRECTION from;
	DIRECTION to;

	LightShape(DIRECTION first, DIRECTION second) : from(first), to(second) {}
};

class LightRay : public Graphical
{
private:
	LightShape _shape;
	L_INTENSITY _intensity;
	L_COLOR _color;

public:
	LightRay(Window& window,
			 const Rect& pos,
			 LightShape shape,
			 L_INTENSITY intensity = L_INTENSITY::MEDIUM,
			 L_COLOR color = L_COLOR::WHITE) : Graphical(window, getPathFromShape(shape), pos),
											   _shape(shape),
										  	   _intensity(intensity),
										  	   _color(color) {setColor(color);}

	L_INTENSITY getIntensity() const {return _intensity;}
	LightShape getShape() const {return _shape;}
	L_COLOR getColor() const {return _color;}

	void setIntensity(const L_INTENSITY& inten) {_intensity = inten;}
	void setColor(const L_COLOR& color);
	void setShape(const LightShape& shape) {_shape = shape; updateTexture(getPathFromShape(shape));}
	void setRect(const Rect& rect) {_rect = rect;}

private:
	std::string getPathFromShape(const LightShape& shape) const;
};

#endif //LIGHT_H