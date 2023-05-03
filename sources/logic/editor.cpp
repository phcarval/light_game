#include "main/configuration.h"
#include "logic/editor.h"
#include "logic/pause_menu.h"
#include "command_input/input_status.h"
#include "graphics/interactive_text_box.h"

void Editor::updateOtherEvents(const InputStatus& status)
{
	if(_textBox != NULL)
	{
		_textBox->updateEvents(status);
		if(_textBox->toBeDestroyed())
		{
			std::string fileName = _textBox->getText();
			if(!fileName.empty())
			{
				fileName = "/home/philippe/Documents/jeu de la mort/custom_levels/" + fileName;
				this->save(fileName);
			}
			delete _textBox;
			_textBox = NULL;
		}
	}
	else
	{
		if(status.getKeyboard().isSaving())
			_textBox = new InteractiveTextBox(_window, {{100,100},{600,200}});
		
		if(status.getKeyboard().isRotating())
		{
			_rotable = !_rotable;
		}
		
		if(status.getKeyboard().isTogglingMovable())
		{
			_movable = !_movable;
		}

		if(status.getMouse().isClicking(M_LEFT))
		{
			placeTile(Tile(_window, Rect(), _curType, _movable, _rotable), status.getMouse().getPosition());
		}

		if(status.getMouse().isScrolling(M_UP))
		{
			if(_curType + 1 < TILE_TYPE::ERROR)
			{
				_curType = static_cast<TILE_TYPE>(_curType+1);

				_selectedTile.updateTexture(Tile::getTexturePathFromType(_curType));
			}
		}
		else if(status.getMouse().isScrolling(M_DOWN))
		{
			if(_curType > 1)
			{
				_curType = static_cast<TILE_TYPE>(_curType-1);
				_selectedTile.updateTexture(Tile::getTexturePathFromType(_curType));
			}
		}
	}
}

Editor::~Editor()
{
	if(_textBox != NULL)
		delete _textBox;
}

void Editor::render()
{
	_window.renderClear();

	renderBackground();
	renderTileChoiceBackground();
	renderLevelTiles();
	renderLights();
	renderTileList();
	_selectedTile.render();

	if(_textBox != NULL)
		_textBox->render();

	if(isPaused())
		getPauseMenu().render();

	_window.renderDraw();
}

void Editor::updateAdditionalTileEvents(Tile& curTile, const InputStatus& status)
{
	if(curTile.isDragging(M_RIGHT) && curTile.getType() != TILE_TYPE::BLANK)
		curTile.clear();
}
