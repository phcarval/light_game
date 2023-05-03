#ifndef LEVEL_H
#define LEVEL_H

#include <map>
#include <set>
#include <vector>
#include <algorithm>

#include "logic/logic_common.h"
#include "logic/tile.h"
#include "main/common.h"
#include "logic/engine.h"
#include "interface/button.h"
#include "main/window_context.h"
#include "logic/main_levels.h"

class PauseMenu;

class Level : public WindowContext //class managing a single level and its grid (tiling) and all that is present on the map
{
public:
	typedef std::map<TILE_TYPE,Tile> TileCount;
	typedef std::vector<std::vector<Tile>> LevelTiling;

	typedef Position WindowPosition;
	typedef Position TilePosition;

	typedef int BUT_EFFECT;

	enum {NONE = -1};

	enum {LIGHTS_ON, LIGHTS_OFF};

private:
	LevelNb _levelNumber;

private:
	Engine _engine;

	PauseMenu* _pauseMenu;
	
protected:
	LevelTiling _tiling;
	Rect _mainTilingScreen;
	Texture _mainTilingScreenTexture;

	TileCount _unplacedTiles;
	Texture _tileChoiceTexture;
	Rect _tileChoiceWindow;

	int _levelWidth;
	int _levelHeight;

	std::set<TilePosition,Position::TileComp> _origin;
	std::set<TilePosition,Position::TileComp> _objective;

	Window& _window;

	std::map<BUT_EFFECT,Button> _buttons;

public:
	const Rect& getMainTilingScreen() const {return _mainTilingScreen;}
	Rect& getMainTilingScreen() {return _mainTilingScreen;}

	const TileCount& getUnplacedTiles() const {return _unplacedTiles;}
	TileCount& getUnplacedTiles() {return _unplacedTiles;}

	const Tile& getTile(const TilePosition& coor) const {return _tiling[coor.y][coor.x];}
	Tile& getTile(const TilePosition& coor) {return _tiling[coor.y][coor.x];}

	bool hasOrigin() const {return !_origin.empty();}
	bool hasDest() const {return !_objective.empty();}

	const Engine& getEngine() const {return _engine;}
	Engine& getEngine() {return _engine;}

	const std::map<BUT_EFFECT,Button>& getButtons() const {return _buttons;}
	std::map<BUT_EFFECT,Button>& getButtons() {return _buttons;}

public: // may (should) be overridden
	virtual bool placeTile(const Tile& tile, const WindowPosition& coordinates);

public:
	const std::set<TilePosition,Position::TileComp>& getOrigin() const {return _origin;}
	const std::set<TilePosition,Position::TileComp>& getObjective() const {return _objective;}

	int getWidth() const {return _levelWidth;}
	int getHeight() const {return _levelHeight;}

	WindowContext* updateContext(const InputStatus& status) override;

	TilePosition getLevelCoordinates(const WindowPosition& coor) const;

	const Window& getWindow() const {return _window;}
	Window& getWindow() {return _window;}

public:
	Level(Window& window, int levelHeight, int levelWidth);
	Level(Window& window, std::vector<std::vector<TILE_TYPE>> tiles,
						  std::vector<TILE_TYPE> uTiles,
						  LevelNb number = SINGLE_LEVEL);
	Level(Window& window, const std::string& path, LevelNb number = SINGLE_LEVEL);

private:
	void init(std::vector<std::vector<TILE_TYPE>> tiles = std::vector<std::vector<TILE_TYPE>>(), std::vector<TILE_TYPE> uTiles = std::vector<TILE_TYPE>());

public:
	bool isPaused() const {return _pauseMenu != NULL;}
	const PauseMenu& getPauseMenu() const {return *_pauseMenu;}
	PauseMenu& getPauseMenu() {return *_pauseMenu;}

	virtual ~Level();

public:
	int getTileSize() const;

private:
	void switchPause();
	Rect calculateNextUnplacedTilePosition() const;

protected:
	void calculateLightPath() {_engine.calculateLightPath();}
	void resetLightPath() {_engine.resetLightPath();}

private:
	void updateKeyboardEvents(const InputStatus& status);
	void updateTilingEvents(const InputStatus& status);

	WindowContext* updatePauseEvents(const InputStatus& status);

protected:
	virtual void updateButtonEvents(const InputStatus& status);

	virtual void updateOtherEvents(const InputStatus& status) {}

	virtual void updateAdditionalTileEvents(Tile& curTile, const InputStatus& status) {}

	virtual WindowContext* onWinning();

/** RENDERING **/

protected:
	virtual void render() override;

	void renderBackground();
	void renderTileChoiceBackground();
	void renderTileList() const;
	void renderLevelTiles() const;
	void renderLights() const;

public:
	void save(const std::string& path) const;

	virtual void serialize(std::ofstream& str) const;
	virtual void deserialize(std::ifstream& str);

private:
	void unlockNextLevel() const;
};

class LevelException : public std::runtime_error
{
public:
	explicit LevelException(const std::string& error) : std::runtime_error(error) {}
	explicit LevelException(const char* error) : std::runtime_error(error) {}
};

#endif //LEVEL_H
