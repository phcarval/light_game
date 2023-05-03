#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "logic/tile.h"
#include "logic/level.h"
#include "file_io/level_load.h"

using namespace std;

TILE_TYPE LevelLoader::parseTile(char c)
{
	TILE_TYPE type;
	switch(c)
	{
		case '+':
			type = TILE_TYPE::BLANK;
			break;

		case '/':
			type = TILE_TYPE::DIAGONAL_URDL;
			break;

		case 'C':
			type = TILE_TYPE::DEST_DOWN;
			break;

		case 'U':
			type = TILE_TYPE::SOURCE_RIGHT;
			break;

		case 'O':
			type = TILE_TYPE::OBSTACLE;
			break;

		default:
			cout << "Symbol not recognized by parser" << endl;
			type = TILE_TYPE::ERROR;
			break;
	}
	return type;
}

pair< vector<vector<TILE_TYPE>>, vector<TILE_TYPE>> LevelLoader::parseLevel()
{
	vector<vector<TILE_TYPE>> tiling;

	vector<TILE_TYPE> tileList;

	char c;
	int i = 0; // x ; y
	bool error = false;

	tiling.push_back(std::vector<TILE_TYPE>());

	while(_levelFile.get(c) && !error && c != '*')
	{
		if(c != '\n')
		{
			TILE_TYPE type = parseTile(c);

			if(type != TILE_TYPE::ERROR)
				tiling[i].push_back(type);
			else
				error = true;
		}
		else
		{
			tiling.push_back(vector<TILE_TYPE>());
			i++;
		}
	}
	if(c == '*') // loading the unplaced tiles available to the player
	{
		while(_levelFile.get(c) && !error)
		{
			tileList.push_back(parseTile(c));
		}
	}
	if(error)
		cout << "Error while loading the level : at least one of the symbols could not be parsed successfully." << endl;

	return {tiling, tileList};
}

Level* LevelLoader::load()
{
	auto lvl = parseLevel();
	return new Level(_window, lvl.first, lvl.second);
}

LevelLoader::LevelLoader(const std::string& path, Window& window) : _path(path), _window(window)
{
	_levelFile.open(path, std::ifstream::in);
	if(!_levelFile.is_open())
	{
		cout << "Error while opening " << _path << endl;
	}
}

LevelLoader::~LevelLoader()
{
	if(_levelFile.is_open())
		_levelFile.close();
}
