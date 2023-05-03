#ifndef LEVEL_LOAD_H
#define LEVEL_LOAD_H

#include <fstream>
#include <utility>
#include <string>

#include "logic/level.h"
#include "main/window.h"

class Tile;

class LevelLoader //class managing loading of a level
{
private:
	std::string _path;
	std::ifstream _levelFile;

	Window& _window;

private:
	TILE_TYPE parseTile(char c);
	std::pair<std::vector<std::vector<TILE_TYPE>>, std::vector<TILE_TYPE>> parseLevel();

public:
	LevelLoader(const std::string& path, Window& window);
	Level* load();
	~LevelLoader();
};

#endif //LEVEL_LOAD_H