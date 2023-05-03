#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <utility>
#include <set>
#include <stdexcept>

#include "logic/logic_common.h"
#include "main/common.h"
#include "logic/light.h"

#define MAX_RECURSIONS 100

class Level;
class Tile;

typedef std::vector<LightRay> LightPath;

class Engine //calculates the resolution of a level (lighting engine)
{
	static const int _max_recursions = MAX_RECURSIONS;
	static int _cur_recursions;

	Level& _level;
	LightPath _lightPath;
	std::set<Position,Position::TileComp> _objectives_complete;

public:
	Engine(Level& level) : _level(level),
						   _lightPath(LightPath()) {}

	void calculateLightPath();

	void resetLightPath() {_lightPath = LightPath(); _objectives_complete.clear();}

private:
	void calculateLightPath(Position curPos, const LightShape& dir, const L_INTENSITY& intensity = MEDIUM, const L_COLOR& color = WHITE);
	
	enum PATH {OBSTACLE, FREE, END};
	
	LightShape getLightShapeFromSource(const Tile& tile) const;
	PATH getNextPos(Position& curPos, LightRay& ray);

private:
	PATH getUpTileStatus(Position& curPos, LightRay& ray);
	PATH getRightTileStatus(Position& curPos, LightRay& ray);
	PATH getLeftTileStatus(Position& curPos, LightRay& ray);
	PATH getDownTileStatus(Position& curPos, LightRay& ray);

public:
	const Level& getLevel() const {return _level;}
	Level& getLevel() {return _level;}

	const LightPath& getLightPath() const {return _lightPath;}
	LightPath& getLightPath() {return _lightPath;}

	bool has_won() const;

	bool has_light_path() const {return !_lightPath.empty();}
};

class EngineException : public std::logic_error
{
public:
	explicit EngineException(const std::string& what) : logic_error(what) {}
	explicit EngineException(const char* what) : logic_error(what) {}
};

#endif
