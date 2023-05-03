#include <exception>
#include <iostream>
#include <algorithm>

#include "main/configuration.h"
#include "main/window.h"

#include "interface/shape.h"
#include "graphics/texture.h"

using namespace std;

Window::Window(const string& name) : _width(WINDOW_WIDTH), _height(WINDOW_HEIGHT), _window(NULL), _renderer(NULL)
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	window = SDL_CreateWindow(name.c_str(),
							 SDL_WINDOWPOS_CENTERED,
							 SDL_WINDOWPOS_CENTERED,
							 WINDOW_WIDTH,
							 WINDOW_HEIGHT,
							 SDL_WINDOW_SHOWN);

	if(window == NULL)
	{
		cout << "Problem while creating window : " << SDL_GetError() << endl;
		throw;
	}
	else
	{
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if(renderer == NULL)
		{
			SDL_DestroyWindow(window);
			throw WindowException(std::string("Problem while creating renderer : ") + SDL_GetError());
		}
		else
		{
			_window = window;
			_renderer = renderer;
		}
	}
}

Window* WindowFactory::createWindow(const string& name)
{
	Window* window = NULL;
	try
	{
		window = new Window(name);
	}
	catch(const WindowException& e)
	{
		cout << e.what() << endl;
		window = NULL;
	}
	return window;
}

void Window::renderSetViewport(const Rect& viewport) const
{
	const SDL_Rect rect = viewport.to_SDL_Rect();
	//std::cout << rect.x << " " << rect.y << " " << rect.w << " " << rect.h << std::endl;
	SDL_RenderSetViewport(_renderer, &rect);
}

void Window::renderRemoveViewport() const
{
	SDL_RenderSetViewport(_renderer, NULL);
}

void Window::renderAddTexture(Texture& text, const Rect& rect) const
{
	SDL_Rect nRect = rect.to_SDL_Rect();
	SDL_Texture* texture = text.getTexture();
	if(texture != NULL)
		SDL_RenderCopy(_renderer, texture, NULL, &nRect);

	texture = text.getTextTexture();
	if(texture != NULL)
		SDL_RenderCopy(_renderer, texture, NULL, &nRect);

	texture = text.getCounterTexture();
	if(texture != NULL)
	{
		SDL_Rect counterRect = {int(nRect.x+(0.75*nRect.w)),
								int(nRect.y+(0.75*nRect.h)),
								int(nRect.w*0.4),
								int(nRect.h*0.4)};
		SDL_RenderCopy(_renderer, texture, NULL, &counterRect);
	}
}

void Window::renderClear() const
{
	SDL_SetRenderDrawColor(_renderer, 215, 215, 215, 0);
	SDL_RenderClear(_renderer);
}

void Window::renderDraw() const
{
	SDL_RenderPresent(_renderer);
}
