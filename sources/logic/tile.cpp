#include "main/configuration.h"

#include "logic/tile.h"
#include "logic/level.h"

#include "command_input/input_status.h"

#include <iostream>
#include <fstream>

using namespace std;

Tile& Tile::operator=(const Tile& other)
{
	Interactive::operator=(other);
	_tileType = other._tileType;
	_nTiles = other._nTiles;
	_rotable = other._rotable;
	return *this;
}

Tile& Tile::operator=(Tile&& other)
{
	Interactive::operator=(other);
	_tileType = other._tileType;
	_nTiles = other._nTiles;
	_rotable = other._rotable;
	return *this;
}

bool Tile::getDraggability(const TILE_TYPE& type) // private : used for constructor
{
	bool drag = true;
	if(type == TILE_TYPE::BLANK || type == TILE_TYPE::UNUSABLE)
		drag = false;
	return drag;
}

string Tile::getTexturePathFromType(const TILE_TYPE& type)
{
	string path = "";
	switch(type)
	{
		case TILE_TYPE::BLANK:
			path = "";
			break;

		case TILE_TYPE::UNUSABLE:
			path = "/home/philippe/Documents/jeu de la mort/assets/unusable.png";
			break;

		case TILE_TYPE::DIAGONAL_ULDR:
			path = "/home/philippe/Documents/jeu de la mort/assets/mirror_1.png";
			break;

		case TILE_TYPE::DIAGONAL_URDL:
			path = "/home/philippe/Documents/jeu de la mort/assets/mirror_2.png";
			break;

		case TILE_TYPE::SPLITTER_DOWN:
			path = "/home/philippe/Documents/jeu de la mort/assets/splitter_down.png";
			break;

		case TILE_TYPE::SPLITTER_UP:
			path = "/home/philippe/Documents/jeu de la mort/assets/splitter_up.png";
			break;

		case TILE_TYPE::SPLITTER_RIGHT:
			path = "/home/philippe/Documents/jeu de la mort/assets/splitter_right.png";
			break;

		case TILE_TYPE::SPLITTER_LEFT:
			path = "/home/philippe/Documents/jeu de la mort/assets/splitter_left.png";
			break;

		case TILE_TYPE::DEST_RIGHT:
			path = "/home/philippe/Documents/jeu de la mort/assets/dest_right.png";
			break;

		case TILE_TYPE::DEST_LEFT:
			path = "/home/philippe/Documents/jeu de la mort/assets/dest_left.png";
			break;

		case TILE_TYPE::DEST_UP:
			path = "/home/philippe/Documents/jeu de la mort/assets/dest_up.png";
			break;

		case TILE_TYPE::DEST_DOWN:
			path = "/home/philippe/Documents/jeu de la mort/assets/dest_down.png";
			break;

		case TILE_TYPE::SOURCE_RIGHT:
			path = "/home/philippe/Documents/jeu de la mort/assets/source_right.png";
			break;

		case TILE_TYPE::SOURCE_LEFT:
			path = "/home/philippe/Documents/jeu de la mort/assets/source_left.png";
			break;

		case TILE_TYPE::SOURCE_UP:
			path = "/home/philippe/Documents/jeu de la mort/assets/source_up.png";
			break;

		case TILE_TYPE::SOURCE_DOWN:
			path = "/home/philippe/Documents/jeu de la mort/assets/source_down.png";
			break;

		case TILE_TYPE::PAINTER_RED_VERTICAL:
			path = "/home/philippe/Documents/jeu de la mort/assets/painter_red_vertical.png";
			break;

		case TILE_TYPE::PAINTER_RED_HORIZONTAL:
			path = "/home/philippe/Documents/jeu de la mort/assets/painter_red_horizontal.png";
			break;

		case TILE_TYPE::PAINTER_GREEN_VERTICAL:
			path = "/home/philippe/Documents/jeu de la mort/assets/painter_green_vertical.png";
			break;

		case TILE_TYPE::PAINTER_GREEN_HORIZONTAL:
			path = "/home/philippe/Documents/jeu de la mort/assets/painter_green_horizontal.png";
			break;

		case TILE_TYPE::PAINTER_BLUE_VERTICAL:
			path = "/home/philippe/Documents/jeu de la mort/assets/painter_blue_vertical.png";
			break;

		case TILE_TYPE::PAINTER_BLUE_HORIZONTAL:
			path = "/home/philippe/Documents/jeu de la mort/assets/painter_blue_horizontal.png";
			break;

		case TILE_TYPE::OBSTACLE:
			path = "/home/philippe/Documents/jeu de la mort/assets/obstacle.png";
			break;

		default:
			path = "";
			cout << "Tile graphics not found" << endl;
			break;
	}
	return path;
}

void Tile::rotate()
{
	if(_rotable)
	{
		bool hasRotated = true;
		switch(_tileType)
		{
			case TILE_TYPE::DEST_RIGHT:
				_tileType = DEST_DOWN;
				break;

			case TILE_TYPE::DEST_DOWN:
				_tileType = DEST_LEFT;
				break;

			case TILE_TYPE::DEST_LEFT:
				_tileType = DEST_UP;
				break;

			case TILE_TYPE::DEST_UP:
				_tileType = DEST_RIGHT;
				break;

			case TILE_TYPE::SOURCE_RIGHT:
				_tileType = SOURCE_DOWN;
				break;

			case TILE_TYPE::SOURCE_DOWN:
				_tileType = SOURCE_LEFT;
				break;

			case TILE_TYPE::SOURCE_LEFT:
				_tileType = SOURCE_UP;
				break;

			case TILE_TYPE::SOURCE_UP:
				_tileType = SOURCE_RIGHT;
				break;

			case TILE_TYPE::DIAGONAL_URDL:
				_tileType = DIAGONAL_ULDR;
				break;

			case TILE_TYPE::DIAGONAL_ULDR:
				_tileType = DIAGONAL_URDL;
				break;

			case TILE_TYPE::SPLITTER_RIGHT:
				_tileType = SPLITTER_DOWN;
				break;

			case TILE_TYPE::SPLITTER_DOWN:
				_tileType = SPLITTER_LEFT;
				break;

			case TILE_TYPE::SPLITTER_LEFT:
				_tileType = SPLITTER_UP;
				break;

			case TILE_TYPE::SPLITTER_UP:
				_tileType = SPLITTER_RIGHT;
				break;

			case TILE_TYPE::PAINTER_BLUE_VERTICAL:
				_tileType = PAINTER_BLUE_HORIZONTAL;
				break;

			case TILE_TYPE::PAINTER_BLUE_HORIZONTAL:
				_tileType = PAINTER_BLUE_VERTICAL;
				break;

			case TILE_TYPE::PAINTER_RED_VERTICAL:
				_tileType = PAINTER_RED_HORIZONTAL;
				break;

			case TILE_TYPE::PAINTER_RED_HORIZONTAL:
				_tileType = PAINTER_RED_VERTICAL;
				break;

			case TILE_TYPE::PAINTER_GREEN_VERTICAL:
				_tileType = PAINTER_GREEN_HORIZONTAL;
				break;

			case TILE_TYPE::PAINTER_GREEN_HORIZONTAL:
				_tileType = PAINTER_GREEN_VERTICAL;
				break;

			default:
				hasRotated = false;
				break;
		}
		if(hasRotated)
			this->updateTexture(this->getTexturePathFromType(_tileType));
	}
}
/*
void Tile::onClick(const InputStatus& status, M_BUTTON but)
{
	if(but == M_RIGHT)
		rotate();
}

void Tile::onDragStop(const InputStatus& status)
{
	if(_lvl.placeTile(*this,status.getMouse().getPosition())) (*this)--;
}
*/

void Tile::setType(TILE_TYPE other)
{
	this->_tileType = other;
	updateTexture(getTexturePathFromType(other));
	_can_be_dragged = getDraggability(other);
	updateColor();
}

void Tile::setType(const Tile& other)
 {
 	this->_tileType = other.getType();
 	updateTexture(getTexturePathFromType(getType())); 
 	_can_be_dragged = other._can_be_dragged;
 	_rotable = other._rotable;
 	updateColor();
 }

void Tile::deserialize(std::ifstream& str)
{
	str.read((char*)&_can_be_dragged, sizeof(_can_be_dragged));
	str.read((char*)&_tileType, sizeof(_tileType));
	str.read((char*)&_nTiles, sizeof(_nTiles));
	str.read((char*)&_rotable, sizeof(_rotable));

	this->updateTexture(getTexturePathFromType(_tileType));
	init();
}

void Tile::serialize(std::ofstream& str) const
{
	str.write((char*)&_can_be_dragged, sizeof(_can_be_dragged));
	str.write((char*)&_tileType, sizeof(_tileType));
	str.write((char*)&_nTiles, sizeof(_nTiles));
	str.write((char*)&_rotable, sizeof(_rotable));
}

void Tile::init()
{
	updateNumberOfTiles();
	updateColor();
}

void Tile::updateColor()
{
	if(!_can_be_dragged && !_rotable)
		setColor(255,100,100);
	else if(!_rotable)
		setColor(100,255,100);
	else if(!_can_be_dragged)
		setColor(100,100,255);
	else
		setColor(255,255,255);
}
