#include <iostream>
#include <vector>

#include "logic/level.h"
#include "main/window.h"

#include "main/configuration.h"
#include "interface/button.h"

#include "logic/pause_menu.h"
#include "logic/level_end_menu.h"
#include "main/main_menu.h"

#include "command_input/input_status.h"

#include "logic/main_levels.h"

#include <fstream>

#define SAVE_MAGIC_V1 0x1

#define UPDATED_SAVE_MAGIC 0x1

using namespace std;

void Level::init(vector<vector<TILE_TYPE>> tiles, vector<TILE_TYPE> uTiles)
{
	_mainTilingScreen = (
		Rect(Position(0,0),
			 Position(_window.getWidth(),MAIN_SCREEN_RATIO*_window.getHeight()))
	);

	_tileChoiceWindow = (
		Rect(Position(0,MAIN_SCREEN_RATIO*_window.getHeight()),
			 Position(_window.getWidth(),_window.getHeight()))
	);

	int tileSize = this->getTileSize();

	for(int i = 0 ; i < _levelHeight ; i++)
	{
		_tiling.push_back(vector<Tile>());
		for(int j = 0 ; j < _levelWidth ; j++)
		{
			TILE_TYPE curType = TILE_TYPE::BLANK;
			if(!tiles.empty())
				curType = tiles[i][j];

			Rect tilePos(j*tileSize, i*tileSize, tileSize, tileSize);
			_tiling[i].push_back(Tile(_window, tilePos, curType));

			if(Tile::isOrigin(curType))
			{
				_origin.insert(Position(j,i));
			}
			else if(Tile::isDest(curType))
			{
				_objective.insert(Position(j,i));
			}
		}
	}
	if(!hasOrigin() || !hasDest())
		std::cout << "Error while creating the level : either the destination or the origin was not found" << std::endl;

	int yOrigin = _window.getHeight()*MAIN_SCREEN_RATIO;
	int yMax = 0;//window.getHeight()*0.9;
	int yIncrement = _window.getHeight()*0.1;

	int xOrigin = 0.5 * tileSize;
	int xMax = _levelWidth-tileSize;
	int xIncrement = (xMax-xOrigin)/10;

	int nTiles = uTiles.size();

	int curTileSize = tileSize/2;

	int x = xOrigin, y = yOrigin;

	for(int i = 0 ; i < nTiles ; i++)
	{
		TILE_TYPE curTileType = uTiles[i];
		if(_unplacedTiles.count(curTileType))
		{
			_unplacedTiles.at(curTileType)++;
		}
		else
		{
			//std::cout << x << " " << y << " " << curTileSize << std::endl;
			_unplacedTiles.insert({curTileType,Tile(_window, Rect(x,y,curTileSize,curTileSize), curTileType)});
			y += yIncrement;
			if(y > yMax)
			{
				x += xIncrement;
				y = yOrigin;
			}
		}
	}
	
	x = _window.getWidth()*(TILING_SCREEN_HORIZONTAL_RATIO + 0.02);
	y = 20;
	_buttons.insert({LIGHTS_ON,Button(_window,Rect(x,y,tileSize,tileSize))});
	_buttons.insert({LIGHTS_OFF,Button(_window,Rect(x,y+2*tileSize,tileSize,tileSize))});
}

Rect Level::calculateNextUnplacedTilePosition() const
{
	int tileSize = this->getTileSize();
	int n = _unplacedTiles.size();

	int yOrigin = _window.getHeight()*MAIN_SCREEN_RATIO;
	int yMax = 0;//window.getHeight()*0.9;
	int yIncrement = _window.getHeight()*0.1;

	// std::cout << yOrigin << " " << yMax << " " << yIncrement << std::endl;

	int xOrigin = 0.5 * tileSize;
	int xMax = _window.getWidth()-tileSize;
	int xIncrement = (xMax-xOrigin)/10;

	int x = xOrigin + (n*xIncrement);

	Rect rect(x,yOrigin,tileSize/2,tileSize/2);

	return rect;
}

Level::Level(Window& window, int levelHeight, int levelWidth) : _window(window),
							   _engine(Engine(*this)),
							   _pauseMenu(nullptr),
							   _levelHeight(levelHeight),
							   _levelWidth(levelWidth),
							   _mainTilingScreenTexture(window,""/*"/home/philippe/Documents/jeu de la mort/assets/background.png"*/),
							   _tileChoiceTexture(window,"/home/philippe/Documents/jeu de la mort/assets/unused_tiles.png"),
							   _levelNumber(SINGLE_LEVEL) {init();}

Level::Level(Window& window, vector<vector<TILE_TYPE>> tiles, vector<TILE_TYPE> uTiles, LevelNb number) : _window(window),
							   _engine(Engine(*this)),
							   _pauseMenu(nullptr),
							   _levelHeight(tiles.size()),
							   _levelWidth(tiles[0].size()),
							   _mainTilingScreenTexture(window,""/*"/home/philippe/Documents/jeu de la mort/assets/background.png"*/),
							   _tileChoiceTexture(window,"/home/philippe/Documents/jeu de la mort/assets/unused_tiles.png"),
							   _levelNumber(number) {init(tiles, uTiles);}

Level::Level(Window& window, const std::string& path, LevelNb number) : _window(window),
														_engine(Engine(*this)),
														_pauseMenu(nullptr),
														_mainTilingScreenTexture(window,""/*"/home/philippe/Documents/jeu de la mort/assets/background.png"*/),
														_tileChoiceTexture(window,"/home/philippe/Documents/jeu de la mort/assets/unused_tiles.png"),
														_levelNumber(number)
{
	ifstream writeStr(path, ios::in | ios::binary);
	if(!writeStr)
	{
		throw LevelException("could not open file " + path + " for loading");
	}
	else
	{
		try
		{
			deserialize(writeStr);
		}
		catch(const LevelException& e)
		{
			std::cout << e.what() << std::endl;
			writeStr.close();
			throw;
		}
		writeStr.close();
	}
}

Level::~Level()
{
 	if(_pauseMenu != nullptr)
 	{
 		delete _pauseMenu;
 		_pauseMenu = nullptr;
 	}
}

Level::TilePosition Level::getLevelCoordinates(const WindowPosition& coor) const
{
	TilePosition newPos(floor(coor.x/this->getTileSize()),floor(coor.y/this->getTileSize()));
	if(newPos.x >= getWidth() || newPos.y >= getHeight())
		newPos = Position(-1,-1);
	return newPos;
}

bool Level::placeTile(const Tile& tile, const WindowPosition& coordinates) // arg is window coordinate
{
	bool success = false;
	if(_mainTilingScreen.in(coordinates))
	{
		TilePosition newCoor = getLevelCoordinates(coordinates);
		if(newCoor.x != -1)
		{
			Tile& chosenTile = getTile(newCoor);
			if(chosenTile.getType() == TILE_TYPE::BLANK)
			{
				chosenTile.setType(tile);

				if(chosenTile.isOrigin())
				{
					_origin.insert(newCoor);
				}
				else if(chosenTile.isDest())
				{
					_objective.insert(newCoor);
				}

				success = true;
			}
		}
		else success = false;
	}
	else if(_tileChoiceWindow.in(coordinates))
	{
		success = true;
		TILE_TYPE type = tile.getType();
		/*
		if(Tile::isOrigin(type))
			_origin = Position(-1,-1);
		else if(Tile::isDest(type))
			_objective = Position(-1,-1);*/

		if(_unplacedTiles.count(type))
		{
			_unplacedTiles.at(type)++;
		}
		else
		{
			Rect pos = calculateNextUnplacedTilePosition();
			_unplacedTiles.insert({type,Tile(_window,pos,type)});
		}
	}
	else success = false;
	return success;
	/*if(success)
		tile--;*/
}

void Level::switchPause()
{
	if(!isPaused())
		_pauseMenu = new PauseMenu(_window, *this);
	else
	{
		delete _pauseMenu;
		_pauseMenu = NULL;
	}
}

int Level::getTileSize() const
{
	int w = this->_window.getWidth()*TILING_SCREEN_HORIZONTAL_RATIO;
	int h = this->_window.getHeight()*MAIN_SCREEN_RATIO; // that's a tempoàrary variable

	int nTilesWide = this->getWidth();
	int nTilesHigh = this->getHeight();

	int tileHeight = h/nTilesHigh;
	int tileWidth = w/nTilesWide;

	return std::min(tileHeight, tileWidth);
}

WindowContext* Level::updateContext(const InputStatus& status)
{
	WindowContext* c = this;
	updateKeyboardEvents(status);
	if(isPaused())
		c = updatePauseEvents(status);
	else
	{
		if(!this->_engine.has_light_path())
		{
			updateTilingEvents(status);
			updateOtherEvents(status);
		}
		updateButtonEvents(status);
	}

	if(_engine.has_won() && c == this)
	{
		c = this->onWinning();
	}

	return c;
}

void Level::unlockNextLevel() const
{
	MainLevels::unlockNewLevel(_levelNumber);
}

WindowContext* Level::onWinning()
{
	unlockNextLevel();
	return new LevelEndMenu(_window, _levelNumber);
}

WindowContext* Level::updatePauseEvents(const InputStatus& status)
{
	WindowContext* c = _pauseMenu->updateContext(status);
	if(c == this)
		switchPause();
	else if(c == _pauseMenu) //do nothing
		c = this;
	return c;
}

void Level::updateKeyboardEvents(const InputStatus& status)
{
	if(status.getKeyboard().isPausing())
		this->switchPause();
}

void Level::updateButtonEvents(const InputStatus& status)
{
	for(auto i = _buttons.begin() ; i != _buttons.end() ; i++)
	{
		i->second.updateEvents(status);
		if(i->second.isClicking(M_LEFT))
		{
			BUT_EFFECT effect = i->first;
			if(effect == LIGHTS_ON) calculateLightPath();
			else if(effect == LIGHTS_OFF) resetLightPath();
		}
	}
}

void Level::updateTilingEvents(const InputStatus& status)
{
	int yDim = _tiling.size();
	int xDim = _tiling[0].size();
	for(int i = 0 ; i < yDim ; i++)
	{
		for(int j = 0 ; j < xDim ; j++)
		{
			Tile& curTile = _tiling[i][j];
			curTile.updateEvents(status);

			updateAdditionalTileEvents(curTile, status);

			if(curTile.isClicking(M_RIGHT))
				curTile.rotate();

			if(curTile.stoppedDragging(M_LEFT))
			{
				if(placeTile(curTile, status.getMouse().getPosition()))
					curTile--;
			}

			if(curTile.toBeDestroyed())
			{
				if(curTile.isOrigin())
				{
					_origin.erase({j,i});
				}
				else if(curTile.isDest())
				{
					_objective.erase({j,i});
				}

				curTile++;
				curTile.setType(TILE_TYPE::BLANK);
				curTile.resetDestruction();
				
			}
		}
	}

	std::vector<TileCount::iterator> itsToDelete;
	for(auto i = _unplacedTiles.begin() ; i != _unplacedTiles.end() ; i++)
	{
		i->second.updateEvents(status);

		updateAdditionalTileEvents(i->second, status);

		if(i->second.stoppedDragging(M_LEFT))
			if(placeTile(i->second, status.getMouse().getPosition()))
				i->second--;

		if(i->second.toBeDestroyed())
			itsToDelete.push_back(i);
	}
	for(int i = 0 ; i < itsToDelete.size() ; i++)
		_unplacedTiles.erase(itsToDelete[i]);
}

void Level::render()
{
	_window.renderClear();
	renderBackground();
	renderTileChoiceBackground();
	renderLevelTiles();
	renderLights();
	renderTileList();

	if(isPaused())
		_pauseMenu->render();
	_window.renderDraw();
}

void Level::renderTileList() const
{
	Level::TileCount tileList = getUnplacedTiles();

	for(auto i = tileList.begin() ; i != tileList.end() ; i++)
	{
		i->second.render();
	}
}

void Level::renderTileChoiceBackground()
{
	_tileChoiceTexture.render(_tileChoiceWindow);
}

void Level::renderBackground()
{
	_mainTilingScreenTexture.render(_mainTilingScreen);
}

void Level::renderLevelTiles() const
{
	int nTilesWide = getWidth();
	int nTilesHigh = getHeight();

	const Tile* upperTile = NULL;

	for(int j = 0 ; j < nTilesHigh ; j++)
	{
		for(int i = 0 ; i < nTilesWide ; i++)
		{
			auto &curTile = getTile(Position(i,j));
			if(curTile.isDragging(M_LEFT))
			{
				upperTile = &curTile;
			}
			else
			{
				curTile.render();
			}
		}
	}
	if(upperTile != NULL) upperTile->render();

	auto buttons = getButtons();
	for(auto i = buttons.begin() ; i != buttons.end() ; i++)
	{
		i->second.render();
	}
}

void Level::renderLights() const
{
	const LightPath& lights = getEngine().getLightPath();

	if(!lights.empty())
	{
		const LightPath& path = getEngine().getLightPath();
		for(int i = 0 ; i < path.size() ; i++)
		{
			path[i].render();
		}
	}
}

void Level::save(const std::string& path) const
{
	ofstream writeStr(path, ios::out | ios::binary);
	if(!writeStr)
	{
		cout << "could not open file for saving" << endl;
	}
	else
	{
		serialize(writeStr);
		writeStr.close();
	}
}

void Level::serialize(ofstream& str) const
{
	u_char magic = UPDATED_SAVE_MAGIC;

	str.write((char*)&magic, sizeof(magic));
	str.write((char*)&_levelWidth, sizeof(_levelWidth));
	str.write((char*)&_levelHeight, sizeof(_levelHeight));

	if(!_tiling.empty())
	{
		for(int i = 0 ; i < _tiling.size() ; i++) // level height
		{
			for(int j = 0 ; j < _tiling[0].size() ; j++) // level width
			{
				_tiling[i][j].serialize(str);
			}
		}
	}

	int sizeUnplacedTiles = _unplacedTiles.size();

	str.write((char*)&sizeUnplacedTiles, sizeof(_levelHeight));

	for(auto i = _unplacedTiles.begin() ; i != _unplacedTiles.end() ; i++)
	{
		i->second.serialize(str);
	}
}

void Level::deserialize(ifstream& str)
{
	u_char magic;

	str.read((char*)&magic, sizeof(magic));

	if(magic==SAVE_MAGIC_V1)
	{
		_mainTilingScreen = (
			Rect(Position(0,0),
				 Position(_window.getWidth(),MAIN_SCREEN_RATIO*_window.getHeight()))
		);

		_tileChoiceWindow = (
			Rect(Position(0,MAIN_SCREEN_RATIO*_window.getHeight()),
				 Position(_window.getWidth(),_window.getHeight()))
		);

		str.read((char*)&_levelWidth, sizeof(_levelWidth));
		str.read((char*)&_levelHeight, sizeof(_levelHeight));

		int tileSize = this->getTileSize();

		for(int i = 0 ; i < _levelHeight ; i++) // level height
		{
			_tiling.push_back(vector<Tile>());
			for(int j = 0 ; j < _levelWidth ; j++) // level width
			{
				Rect tilePos(j*tileSize, i*tileSize, tileSize, tileSize);
				_tiling[i].push_back(Tile(_window,tilePos,str));

				if(_tiling[i][j].isOrigin())
				{
					_origin.insert(Position(j,i));
				}
				else if(_tiling[i][j].isDest())
				{
					_objective.insert(Position(j,i));
				}
			}
		}
		int sizeUnplacedTiles;

		str.read((char*)&sizeUnplacedTiles, sizeof(sizeUnplacedTiles));
		for(int i = 0 ; i < sizeUnplacedTiles ; i++)
		{
			Tile nTile(_window, calculateNextUnplacedTilePosition(), str);
			_unplacedTiles.insert({nTile.getType(),std::move(nTile)});
		}
		int x = _window.getWidth() - 3*tileSize;
		int y = 20;
		_buttons.insert({LIGHTS_ON,Button(_window,Rect(x,y,tileSize,tileSize))});
		_buttons.insert({LIGHTS_OFF,Button(_window,Rect(x,y+2*tileSize,tileSize,tileSize))});
	}
	else
	{
		throw LevelException("Unknown loading type error! Magic not recognized");
	}
}
