#ifndef TILE_H
#define TILE_H

#include "logic/logic_common.h"
#include "interface/shape.h"
#include "interface/interactive.h"

class Tile : public Interactive // child of class Interactive
{
private:
	TILE_TYPE _tileType;
	int _nTiles; // if -1 then infinite tiles / not countable
	bool _rotable;

public:
	Tile(Window& window,
		const Rect& rect,
		TILE_TYPE type = TILE_TYPE::BLANK,
		bool draggable = true,
		bool rotable = true,
		int nTiles = 1) : Interactive(window, getTexturePathFromType(type), rect, draggable && getDraggability(type)),
						   _tileType(type),
						   _nTiles(nTiles),
						   _rotable(rotable) {init();}

	Tile(Window& window,
		const Rect& rect,
		std::ifstream& str) : Interactive(window, "", rect, false) {deserialize(str);}

private:
	void init();

public:
	TILE_TYPE getType() const {return _tileType;}

	bool isDest() const {return isDest(this->_tileType);}
	bool isOrigin() const {return isOrigin(this->_tileType);}

	static bool isDest(const TILE_TYPE& type) {return (type == TILE_TYPE::DEST_RIGHT ||
													   type == TILE_TYPE::DEST_LEFT ||
													   type == TILE_TYPE::DEST_UP ||
													   type == TILE_TYPE::DEST_DOWN);}

	static bool isOrigin(const TILE_TYPE& type) {return (type == TILE_TYPE::SOURCE_RIGHT ||
													     type == TILE_TYPE::SOURCE_LEFT ||
													     type == TILE_TYPE::SOURCE_UP ||
													     type == TILE_TYPE::SOURCE_DOWN);}

	~Tile() {}

	Tile(const Tile& other) : Interactive(other),
							  _tileType(other._tileType),
							  _nTiles(other._nTiles),
							  _rotable(other._rotable) {}
	Tile(Tile&& other) : Interactive(other),
						 _tileType(other._tileType),
						 _nTiles(other._nTiles),
						 _rotable(other._rotable) {}

	Tile& operator=(const Tile& other);
	Tile& operator=(Tile&& other);

	void addTile() {if(_nTiles>=0) {_nTiles++; updateNumberOfTiles();} }
	void removeTile() {if(_nTiles>0) {_nTiles--; updateNumberOfTiles();} if(_nTiles==0) _to_be_destroyed = true; }

	Tile& operator++(int) {addTile(); return *this;}
	Tile& operator--(int) {removeTile(); return *this;}

	void setType(const Tile& other);
	void setType(TILE_TYPE type);

	int getNTiles() const {return _nTiles;}

	void rotate();

	void clear() {_nTiles = 0; removeTile();}

public:
	static std::string getTexturePathFromType(const TILE_TYPE& type);
	static bool getDraggability(const TILE_TYPE& type);

/*protected: // virtual implementation
	void onDragStop(const InputStatus& status) override;
	void onClick(const InputStatus& status, M_BUTTON but) override;*/

private:
	void updateNumberOfTiles() {if(_nTiles <= 1) updateCounter(""); else updateCounter(std::to_string(_nTiles));}
	void updateColor();

public:
	virtual void serialize(std::ofstream& str) const;
	virtual void deserialize(std::ifstream& str);
};

#endif //TILE_H