#include <iostream>

#include "logic/logic_common.h"
#include "logic/light.h"

std::string LightRay::getPathFromShape(const LightShape& shape) const
{
	std::string str = "";
	std::string defPath = "/home/philippe/Documents/jeu de la mort/assets/";

	switch(shape.from)
	{
		case UP:
			switch(shape.to)
			{
				case UP:
					str = defPath + "light_up.png";
					break;

				case DOWN:
					str = defPath + "light_updown.png";
					break;

				case LEFT:
					str = defPath + "light_upleft.png";
					break;

				case RIGHT:
					str = defPath + "light_upright.png";
					break;
			}
			break;

		case DOWN:
			switch(shape.to)
			{
				case UP:
					str = defPath + "light_updown.png";
					break;

				case DOWN:
					str = defPath + "light_down.png";
					break;

				case LEFT:
					str = defPath + "light_downleft.png";
					break;

				case RIGHT:
					str = defPath + "light_downright.png";
					break;
			}
			break;
		case RIGHT:
			switch(shape.to)
			{
				case UP:
					str = defPath + "light_upright.png";
					break;

				case DOWN:
					str = defPath + "light_downright.png";
					break;

				case LEFT:
					str = defPath + "light_leftright.png";
					break;

				case RIGHT:
					str = defPath + "light_right.png";
					break;
			}
			break;
		case LEFT:
			switch(shape.to)
			{
				case UP:
					str = defPath + "light_upleft.png";
					break;

				case DOWN:
					str = defPath + "light_downleft.png";
					break;

				case LEFT:
					str = defPath + "light_left.png";
					break;

				case RIGHT:
					str = defPath + "light_leftright.png";
					break;
			}
			break;
	}

	return str;
}

void LightRay::setColor(const L_COLOR& color)
{
	_color = color;
	switch(color)
	{
		case WHITE:
			Graphical::setColor(255,255,255);
			break;

		case BLUE:
			Graphical::setColor(0,0,255);
			break;

		case GREEN:
			Graphical::setColor(0,255,0);
			break;

		case RED:
			Graphical::setColor(255,0,0);
			break;

		default:
			std::cout << "Warning : unknown color" << std::endl;
			Graphical::setColor(255,255,255);
	}
}
