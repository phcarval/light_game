#include "SDL2/SDL.h"
#include "SDL2/SDL_mouse.h"

#include "main/configuration.h"
#include "command_input/input_status.h"

#include <iostream>

void InputStatus::updateEvents()
{
	Mouse& mouse = this->getMouse();
	Keyboard& keyboard = this->getKeyboard();

	mouse.initRoutine();
	keyboard.initRoutine();

	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_MOUSEWHEEL:
				if(event.wheel.y > 0)
					mouse.scroll(M_UP);
				else
					mouse.scroll(M_DOWN);
				break;

			case SDL_KEYDOWN:
				keyboard.addKey(event.key.keysym.sym);
				break;

			case SDL_KEYUP:
				keyboard.removeKey(event.key.keysym.sym);
				break;

			case SDL_QUIT:
				_quit = true;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					mouse.startCountingTime(M_LEFT);
					mouse.startPressing(M_LEFT);
				}
				if(event.button.button == SDL_BUTTON_RIGHT)
				{
					mouse.startCountingTime(M_RIGHT);
					mouse.startPressing(M_RIGHT);
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					mouse.stopPressing(M_LEFT);
					if(!mouse.isHolding(M_LEFT))
						mouse.startClicking(M_LEFT);
					else
						mouse.stopHolding(M_LEFT);
				}
				if(event.button.button == SDL_BUTTON_RIGHT)
				{
					mouse.stopPressing(M_RIGHT);
					if(!mouse.isHolding(M_RIGHT))
						mouse.startClicking(M_RIGHT);
					else
						mouse.stopHolding(M_RIGHT);
				}
				break;

			case SDL_TEXTINPUT:
				keyboard.updateTextInput(event.text.text);

			/*case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					mouse.startLeftClicking();
					mouse.startLeftHolding();
				}
				if(event.button.button == SDL_BUTTON_RIGHT)
				{
					mouse.startRightClicking();
					mouse.startRightHolding();
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT)
					mouse.stopLeftHolding();
				if(event.button.button == SDL_BUTTON_RIGHT)
					mouse.stopRightHolding();
				break;*/

			case SDL_MOUSEMOTION:
				int x,y;
				SDL_GetMouseState(&x, &y);
				mouse.setPosition(Position(x,y));
				break;

			default:
				break;
		}
	}
}

void InputStatus::Mouse::countTimePassed(M_BUTTON button)
{
	using namespace std::chrono;

	hi_res_clock::time_point releaseTime = hi_res_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(releaseTime-_clickTime[button]);

	int milliseconds = int(time_span.count()*1000.0);
	if(milliseconds > _clickDurationBeforeDrag)
	{
		stopPressing(button);
		startHolding(button);
	}
}

void InputStatus::Mouse::measureDistance(M_BUTTON button)
{
	double d = Utils::distance(_position, _clickPosition[button]);
	if(d > _distanceBeforeDrag)
	{
		stopPressing(button);
		startHolding(button);
	}
}

void InputStatus::Mouse::initRoutine()
{
	stopClicking();
	stopScrolling();

	if(_isPressing[M_LEFT])
	{
		countTimePassed(M_LEFT);
		measureDistance(M_LEFT);
	}
	
	if(_isPressing[M_RIGHT])
	{
		countTimePassed(M_RIGHT);
		measureDistance(M_RIGHT);
	}
}

void InputStatus::Keyboard::initRoutine()
{
	unpressButtons();
	resetTextInput();
}
