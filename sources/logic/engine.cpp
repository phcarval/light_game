#include <iostream>

#include "logic/engine.h"
#include "logic/level.h"

int Engine::_cur_recursions = 0;

LightShape Engine::getLightShapeFromSource(const Tile& tile) const
{
	TILE_TYPE type = tile.getType();
	DIRECTION dir;
	switch(type)
	{
		case SOURCE_RIGHT:
			dir = RIGHT;
			break;

		case SOURCE_LEFT:
			dir = LEFT;
			break;

		case SOURCE_UP:
			dir = UP;
			break;

		case SOURCE_DOWN:
			dir = DOWN;
			break;

		default:
			std::cout << "PROBLEM : Unknown source direction : " << tile.getType() << std::endl;
			dir = DOWN;
			break;
	}
	return {dir,dir};
}

Engine::PATH Engine::getNextPos(Position& curPos, LightRay& ray)
{
	Engine::PATH pathStatus = Engine::PATH::FREE;
	auto curDir = ray.getShape().to;
	switch(curDir)
	{
		case UP:
			pathStatus = getUpTileStatus(curPos, ray);
			break;

		case DOWN:
			pathStatus = getDownTileStatus(curPos, ray);
			break;

		case LEFT:
			pathStatus = getLeftTileStatus(curPos, ray);
			break;

		case RIGHT:
			pathStatus = getRightTileStatus(curPos, ray);
			break;
	}
	return pathStatus;
}

void Engine::calculateLightPath(Position curPos, const LightShape& dir, const L_INTENSITY& intensity, const L_COLOR& color)
{
	_cur_recursions++;

	if(_cur_recursions == _max_recursions)
		throw EngineException("Light Path calculation has reached recursion limit (" + std::to_string(_max_recursions) + ")");

	const Level& lvl = getLevel();

	int tileSize = lvl.getTileSize();

	Position truePos(curPos.x*tileSize, curPos.y*tileSize);

	LightRay curLight(_level.getWindow(),
					  {truePos.x,truePos.y,tileSize,tileSize},
					  dir,
					  intensity,
					  color);

	PATH curPathStatus = PATH::FREE;

	while(curPathStatus == PATH::FREE)
	{
		_lightPath.push_back(curLight);
		curPathStatus = getNextPos(curPos, curLight);

		truePos = {curPos.x*tileSize,curPos.y*tileSize};
		curLight.setRect({truePos.x,truePos.y,tileSize,tileSize});
	}
	if(curPathStatus == PATH::END)
	{
		_lightPath.push_back(curLight);
		this->_objectives_complete.insert(curPos);
	}
	else; // path == OBSTACLE : do nothing
}

bool Engine::has_won() const
{
	return _objectives_complete.size() >= 1 && _objectives_complete == _level.getObjective();
}

void Engine::calculateLightPath()
{
	_cur_recursions = 0;
	const Level& lvl = getLevel();

	if(lvl.hasOrigin() && lvl.hasDest())
	{
		for(Position origin : lvl.getOrigin())
		{
			LightShape curDirection = getLightShapeFromSource(lvl.getTile(origin));

			calculateLightPath(origin, curDirection);
		}
	}
	else
	{
		std::cout << "Level has not got an origin and/or a destination" << std::endl;
	}
}

Engine::PATH Engine::getUpTileStatus(Position& curPos, LightRay& ray)
{
	PATH pathStatus = PATH::FREE;
	const Level& lvl = getLevel();
	DIRECTION curDir = DIRECTION::UP;

	if(curPos.y == 0) // it is the edge of the map
		pathStatus = PATH::OBSTACLE;
	else
	{
		curPos.y--;
		TILE_TYPE curTileType = lvl.getTile(curPos).getType(); //what is the tile type at our new position?
		if(curTileType == TILE_TYPE::DEST_DOWN) // if it is a destination oriented down, we won
		{
			pathStatus = PATH::END;
			curDir = DIRECTION::DOWN;
		}

		else if(curTileType == DIAGONAL_ULDR) // the ray will go to the left
			curDir = DIRECTION::LEFT;

		else if(curTileType == DIAGONAL_URDL)
			curDir = DIRECTION::RIGHT;

		else if(curTileType == PAINTER_RED_VERTICAL)
		{
			curDir = DIRECTION::UP;
			ray.setColor(L_COLOR::RED);
		}
		else if(curTileType == PAINTER_GREEN_VERTICAL)
		{
			curDir = DIRECTION::UP;
			ray.setColor(L_COLOR::GREEN);
		}
		else if(curTileType == PAINTER_BLUE_VERTICAL)
		{
			curDir = DIRECTION::UP;
			ray.setColor(L_COLOR::BLUE);
		}

		else if(curTileType == SPLITTER_DOWN)
		{
			curDir = DIRECTION::LEFT;
			calculateLightPath(curPos,{DIRECTION::DOWN,DIRECTION::RIGHT}, ray.getIntensity(), ray.getColor());
		}

		else if(curTileType == TILE_TYPE::BLANK || curTileType == TILE_TYPE::UNUSABLE) //OK, empty tile
			curDir = DIRECTION::UP;

		else // there is some kind of obstacle on the way
			pathStatus = PATH::OBSTACLE;

		ray.setShape({DIRECTION::DOWN, curDir});
	}
	return pathStatus;
}

Engine::PATH Engine::getDownTileStatus(Position& curPos, LightRay& ray)
{
	PATH pathStatus = PATH::FREE;
	const Level& lvl = getLevel();
	DIRECTION curDir = DIRECTION::DOWN;

	if(curPos.y == _level.getHeight()-1) // it is the edge of the map
		pathStatus = PATH::OBSTACLE;
	else
	{
		curPos.y++;
		TILE_TYPE curTileType = lvl.getTile(curPos).getType(); //what is the tile type at our new position?
		if(curTileType == TILE_TYPE::DEST_UP) // if it is a destination oriented down, we won
		{
			pathStatus = PATH::END;
			curDir = DIRECTION::UP;
		}

		else if(curTileType == DIAGONAL_ULDR)
			curDir = DIRECTION::RIGHT;

		else if(curTileType == DIAGONAL_URDL)
			curDir = DIRECTION::LEFT;

		else if(curTileType == SPLITTER_UP)
		{
			curDir = DIRECTION::LEFT;
			calculateLightPath(curPos,{DIRECTION::UP,DIRECTION::RIGHT}, ray.getIntensity(), ray.getColor());
		}

		else if(curTileType == PAINTER_RED_VERTICAL)
		{
			curDir = DIRECTION::DOWN;
			ray.setColor(L_COLOR::RED);
		}
		else if(curTileType == PAINTER_GREEN_VERTICAL)
		{
			curDir = DIRECTION::DOWN;
			ray.setColor(L_COLOR::GREEN);
		}
		else if(curTileType == PAINTER_BLUE_VERTICAL)
		{
			curDir = DIRECTION::DOWN;
			ray.setColor(L_COLOR::BLUE);
		}

		else if(curTileType == TILE_TYPE::BLANK || curTileType == TILE_TYPE::UNUSABLE) //OK, empty tile
			curDir = DIRECTION::DOWN;

		else // there is some kind of obstacle on the way
			pathStatus = PATH::OBSTACLE;

		ray.setShape({DIRECTION::UP, curDir});
	}
	return pathStatus;
}

Engine::PATH Engine::getLeftTileStatus(Position& curPos, LightRay& ray)
{
	PATH pathStatus = PATH::FREE;
	const Level& lvl = getLevel();
	DIRECTION curDir = DIRECTION::LEFT;

	if(curPos.x == 0) // it is the edge of the map
		pathStatus = PATH::OBSTACLE;
	else
	{
		curPos.x--;
		TILE_TYPE curTileType = lvl.getTile(curPos).getType(); //what is the tile type at our new position?
		if(curTileType == TILE_TYPE::DEST_RIGHT) // if it is a destination oriented down, we won
		{
			pathStatus = PATH::END;
			curDir = DIRECTION::RIGHT;
		}

		else if(curTileType == DIAGONAL_ULDR)
			curDir = DIRECTION::UP;

		else if(curTileType == DIAGONAL_URDL)
			curDir = DIRECTION::DOWN;

		else if(curTileType == SPLITTER_RIGHT)
		{
			curDir = DIRECTION::UP;
			calculateLightPath(curPos,{DIRECTION::RIGHT,DIRECTION::DOWN}, ray.getIntensity(), ray.getColor());
		}

		else if(curTileType == PAINTER_RED_HORIZONTAL)
		{
			curDir = DIRECTION::LEFT;
			ray.setColor(L_COLOR::RED);
		}
		else if(curTileType == PAINTER_GREEN_HORIZONTAL)
		{
			curDir = DIRECTION::LEFT;
			ray.setColor(L_COLOR::GREEN);
		}
		else if(curTileType == PAINTER_BLUE_HORIZONTAL)
		{
			curDir = DIRECTION::LEFT;
			ray.setColor(L_COLOR::BLUE);
		}

		else if(curTileType == TILE_TYPE::BLANK || curTileType == TILE_TYPE::UNUSABLE) //OK, empty tile
			curDir = DIRECTION::LEFT;

		else // there is some kind of obstacle on the way
			pathStatus = PATH::OBSTACLE;

		ray.setShape({DIRECTION::RIGHT, curDir});
	}
	return pathStatus;
}

Engine::PATH Engine::getRightTileStatus(Position& curPos, LightRay& ray)
{
	PATH pathStatus = PATH::FREE;
	const Level& lvl = getLevel();
	DIRECTION curDir = DIRECTION::RIGHT;

	if(curPos.x == _level.getWidth()-1) // it is the edge of the map
		pathStatus = PATH::OBSTACLE;
	else
	{
		curPos.x++;
		TILE_TYPE curTileType = lvl.getTile(curPos).getType(); //what is the tile type at our new position?
		if(curTileType == TILE_TYPE::DEST_LEFT) // if it is a destination oriented down, we won
		{
			pathStatus = PATH::END;
			curDir = DIRECTION::LEFT;
		}

		else if(curTileType == DIAGONAL_ULDR)
			curDir = DIRECTION::DOWN;

		else if(curTileType == DIAGONAL_URDL)
			curDir = DIRECTION::UP;

		else if(curTileType == SPLITTER_LEFT)
		{
			curDir = DIRECTION::UP;
			calculateLightPath(curPos,{DIRECTION::LEFT,DIRECTION::DOWN}, ray.getIntensity(), ray.getColor());
		}

		else if(curTileType == PAINTER_RED_HORIZONTAL)
		{
			curDir = DIRECTION::RIGHT;
			ray.setColor(L_COLOR::RED);
		}
		else if(curTileType == PAINTER_GREEN_HORIZONTAL)
		{
			curDir = DIRECTION::RIGHT;
			ray.setColor(L_COLOR::GREEN);
		}
		else if(curTileType == PAINTER_BLUE_HORIZONTAL)
		{
			curDir = DIRECTION::RIGHT;
			ray.setColor(L_COLOR::BLUE);
		}

		else if(curTileType == TILE_TYPE::BLANK || curTileType == TILE_TYPE::UNUSABLE) //OK, empty tile
			curDir = DIRECTION::RIGHT;

		else // there is some kind of obstacle on the way
			pathStatus = PATH::OBSTACLE;

		ray.setShape({DIRECTION::LEFT, curDir});
	}
	return pathStatus;
}
