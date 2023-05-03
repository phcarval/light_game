#include "main/configuration.h"
#include "interface/interactive.h"
#include "command_input/input_status.h"

#include <iostream>

using namespace std;
/*
void Interactive::updateEvents(const InputStatus& status)
{
	//_is
	// cout << status.getMouse().getPosition().x << " " << status.getMouse().getPosition().y << endl;

	if(this->_is_dragged)
	{
		if(status.getMouse().isClicking(M_RIGHT))
		{
			//onRightSelect(status);
			_is_right_selected = true;
		}
		else
			_is_right_selected = false;
		if(status.getMouse().isHolding(M_LEFT)) // thing is currently being dragged
		{
			_graphicsForDragging = new Graphical(*this);
			_graphicsForDragging->updateText("");

			Rect& oldRect = _graphicsForDragging->getRect();
			Position newPos = status.getMouse().getPosition();

			int width = oldRect.getBottomRightCoor().x-oldRect.getTopLeftCoor().x;
			int height = oldRect.getBottomRightCoor().y-oldRect.getTopLeftCoor().y;
			oldRect = Rect(newPos.x-(width/2),newPos.y-(height/2),width,height);
		}
		else // we just released the mouse button
		{
			delete _graphicsForDragging;
			_graphicsForDragging = NULL;
			_is_dragged = false;
			//this->onDragStop(status);
		}
	}
	else // thing was not being dragged
	{
		if(this->getRect().in(status.getMouse().getPosition())) // the mouse is hovering over the object
		{
			_is_hovered = true;
			//onHover(status);

			if(status.getMouse().isPressing(M_LEFT))
			{
				_is_clicked = true;
			}

			if(status.getMouse().isClicking(M_RIGHT))
			{
				//onRightSelect(status);
				_is_right_selected = true;
			}
			else
				_is_right_selected = false;

			if(_is_clicked && status.getMouse().isHolding(M_LEFT) && _can_be_dragged) // we are trying to drag the object
			{
				_is_hovered = false;
				_is_dragged = true;
				_is_clicked = false;
			}

			if(status.getMouse().isClicking(M_LEFT))
			{
				_is_left_selected = true;
				//onLeftSelect(status);
			}
			else
				_is_left_selected = false;
		}
		else // the mouse is not doing anything to the object
		{
			_is_dragged = false;
			_is_left_selected = false;
			_is_hovered = false;
			_is_clicked = false;
		}
	}
	manageKeyboard(status);
}*/

void Interactive::dragCheck(M_BUTTON but, const InputStatus& status)
{
	if(this->isDragging(but))
	{
		if(!status.getMouse().isHolding(but)) // is not being dragged anymore
		{
			if(but == M_LEFT)
			{
				if(_graphicsForDragging != nullptr)
					delete _graphicsForDragging;
				_graphicsForDragging = nullptr;
			}
			stopDragging(but);
			//onDragStop(status);
		}
		else if(but == M_LEFT)
		{
			_graphicsForDragging = new Graphical(*this);
			_graphicsForDragging->updateText("");

			Rect& oldRect = _graphicsForDragging->getRect();
			Position newPos = status.getMouse().getPosition();

			int width = oldRect.getBottomRightCoor().x-oldRect.getTopLeftCoor().x;
			int height = oldRect.getBottomRightCoor().y-oldRect.getTopLeftCoor().y;
			oldRect = Rect(newPos.x-(width/2),newPos.y-(height/2),width,height);
		}
	}
}

void Interactive::checkClickEvents(M_BUTTON but, const InputStatus& status)
{
	if(status.getMouse().isPressing(but)) // if the button has been pressed
	{
		startSelecting(but); // we start selecting the object
	}
	else if(status.getMouse().isClicking(but))
	{
		startClicking(but);
		stopSelecting(but);
		//onClick(status,but);
	}
	else if(status.getMouse().isHolding(but) && _is_selected[but] && (_can_be_dragged || but == M_RIGHT)) // if the object is being selected (button pressed over the object)
	{
		startDragging(but); // we start dragging the object
		stopHovering();
		stopSelecting(but);
	}
	else
	{
		stopSelecting(but);
		stopClicking(but);
	}
}

void Interactive::updateEvents(const InputStatus& status)
{
	_stoppedDragging = {false};
	dragCheck(M_RIGHT, status);
	dragCheck(M_LEFT, status);

	if(this->getRect().in(status.getMouse().getPosition()))
	{
		if(!isDragging(M_RIGHT) && !isDragging(M_LEFT))
		{
			startHovering();
			//onHover(status);
		}
		if(!isDragging(M_RIGHT))
			checkClickEvents(M_RIGHT, status);
		if(!isDragging(M_LEFT))
			checkClickEvents(M_LEFT, status);
	}
	else
	{
		stopHovering();
		stopSelecting(M_RIGHT);
		stopClicking(M_RIGHT);
		stopSelecting(M_LEFT);
		stopClicking(M_LEFT);
	}
	manageKeyboard(status);
}

Interactive& Interactive::operator=(const Interactive& other)
{
	Graphical::operator=(other);

	if(_graphicsForDragging != NULL)
	{
		delete _graphicsForDragging;
		_graphicsForDragging = NULL;
	}

	if(other._graphicsForDragging != NULL)
	{
		_graphicsForDragging = new Graphical(*other._graphicsForDragging);
	}

	_can_be_dragged = other._can_be_dragged;

	_is_dragged = other._is_dragged;
	_is_selected = other._is_selected;
	_is_hovered = other._is_hovered;
	_stoppedDragging = other._stoppedDragging;

	_to_be_destroyed = other._to_be_destroyed;

	_is_clicked = other._is_clicked;

	return *this;
}

Interactive& Interactive::operator=(Interactive&& other)
{
	Graphical::operator=(other);

	_graphicsForDragging = other._graphicsForDragging;
	other._graphicsForDragging = NULL;

	_can_be_dragged = other._can_be_dragged;

	_is_dragged = other._is_dragged;
	_is_selected = other._is_selected;
	_is_hovered = other._is_hovered;
	_stoppedDragging = other._stoppedDragging;

	_to_be_destroyed = other._to_be_destroyed;

	_is_clicked = other._is_clicked;

	return *this;
}

void Interactive::render() const
{
	if(_graphicsForDragging != nullptr)
		_graphicsForDragging->render();
	else
		Graphical::render();
}

/*
Interactive& Interactive::operator=(const Interactive& other)
{
	Graphical::operator=(other);

	_is_hovered = other._is_hovered;
	_is_dragged = other._is_dragged;
	_is_clicked = other._is_clicked;
	_is_selected = other._is_selected;

	return *this;
}

Interactive& Interactive::operator=(Interactive&& other)
{
	Graphical::operator=(other);

	_is_hovered = other._is_hovered;
	_is_dragged = other._is_dragged;
	_is_clicked = other._is_clicked;
	_is_selected = other._is_selected;

	return *this;
}*/
