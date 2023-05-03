#ifndef BASE_GAME_H
#define BASE_GAME_H

#include <unordered_set>
#include <string>

class Window;

class BaseGame
{
private:
	static bool init_SDL_subsystems();
	static void launchMainMenu(Window& window);

	static void quit_SDL_subsystems();

public:
	static void launchBaseGame();
};

#endif