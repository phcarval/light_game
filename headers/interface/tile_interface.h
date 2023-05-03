#ifndef TILE_INTERFACE_H
#define TILE_INTERFACE_H

class Tile_Interface : public Draggable, public Selectable // you can drag and select (hover) tiles
{
	void doAction();
};

#endif //TILE_INTERFACE_H