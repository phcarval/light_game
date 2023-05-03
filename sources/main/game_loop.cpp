#include "SDL2/SDL.h"

#include "main/game_loop.h"
#include "command_input/input_status.h"
#include "main/main_menu.h"

void GameLoop::begin(Window& window)
{
	InputStatus status;

	WindowContext* context = new MainMenu(window);

	bool quit = false;
	while(!quit)
	{
		status.updateEvents();

		if(status.isQuitting())
			quit = true;
		else
		{
			WindowContext* newContext = context->updateContext(status);
			context->render();
			if(newContext == NULL)
				quit = true;
			else if(newContext != context)
			{
				delete context;
				context = newContext;
			}
			
		}
	}
	if(context != NULL)
	{
		delete context;
	}
}
