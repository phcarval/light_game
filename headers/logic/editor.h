#ifndef EDITOR_H
#define EDITOR_H

#include "logic/level.h"
#include "main/window.h"
#include "main/configuration.h"

class InteractiveTextBox;

class Editor : public Level
{
private:
	Graphical _selectedTile;
	TILE_TYPE _curType;

	InteractiveTextBox* _textBox;

	bool _rotable;
	bool _movable;

public:
	Editor(Window& window, int width = 10, int height = 10) : Level(window,
								   width,
								   height),
								   _curType(TILE_TYPE::OBSTACLE),
					 		       _selectedTile(window,
					 			   Tile::getTexturePathFromType(_curType),
					 			   Rect(window.getWidth()*TILING_SCREEN_HORIZONTAL_RATIO,window.getHeight()/2,this->getTileSize(),this->getTileSize())),
					 			   _textBox(NULL),
					 			   _rotable(true),
					 			   _movable(true) {}

	~Editor();

protected:
	WindowContext* onWinning() override {std::cout << "y" << std::endl; return this;}

private:
	void updateOtherEvents(const InputStatus& status) override;

	void render() override;

	void updateAdditionalTileEvents(Tile& curTile, const InputStatus& status) override;
};

#endif //EDITOR_H