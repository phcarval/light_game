#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "main/configuration.h"
#include "main/base_game.h"
//#include "main/main_menu.h"
#include "main/window.h"
#include "main/game_loop.h"

#include <iostream>
#include <exception>

using namespace std;

void BaseGame::launchBaseGame()
{
	if(init_SDL_subsystems())
	{
		Window* window = NULL;
		window = WindowFactory::createWindow("myGame");
		if(window != NULL)
		{
			launchMainMenu(*window);
			delete window;
		}
		else
		{
			cout << "Window creation problems!" << endl;
		}
		quit_SDL_subsystems();
	}
}

bool BaseGame::init_SDL_subsystems() //initializes all SDL subsystems
{
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cout << "SDL initialization failed : " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		{
			cout << "SDL_Image initialization failed : " << IMG_GetError() << endl;
			success = false;
		}
		else
		{
			if(TTF_Init() != 0)
			{
				cout << "SDL_TTF initialization failed : " << TTF_GetError() << endl;
				success = false;
			}
		}
	}
	return success;
}

void BaseGame::quit_SDL_subsystems()
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void BaseGame::launchMainMenu(Window& window)
{
	GameLoop::begin(window);
}
