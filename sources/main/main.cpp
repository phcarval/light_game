#include "main/base_game.h"

#include <iostream>

int main(int argc, char* argv[])
{
	BaseGame::launchBaseGame();
	std::cout << "Exited successfully." << std::endl;
	return 0;
}