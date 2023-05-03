#ifndef SURFACE_H
#define SURFACE_H

#include "SDL2/SDL.h"

class Surface //Wrapper class for manipulating SDL_Surface
{
protected:
	SDL_Surface* _surface;

public:
	Surface(SDL_Surface* s) : _surface(s) {}
	~Surface() {SDL_FreeSurface(_surface);}

public:
	SDL_Surface* getSurface() {return _surface;}
};

#endif //SURFACE_H
