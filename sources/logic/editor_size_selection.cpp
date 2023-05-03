#include "logic/editor_size_selection.h"
#include "logic/editor.h"
#include "command_input/input_status.h"
#include "main/main_menu.h"

#define MAX_DIM 35

WindowContext* EditorSizeSelection::updateContext(const InputStatus& status)
{
	WindowContext* newContext = this;
	for(TextBox& dim : _dimensions)
	{
		dim.updateEvents(status);
		if(dim.isClicking(M_RIGHT))
		{
			int curValue = std::stoi(dim.getText());
			if(curValue > 2)
			{
				curValue--;
				dim.setText(std::to_string(curValue));
			}
		}
		else if(dim.isClicking(M_LEFT))
		{
			int curValue = std::stoi(dim.getText());
			if(curValue < MAX_DIM)
			{
				curValue++;
				dim.setText(std::to_string(curValue));
			}
		}
	}

	if(status.getKeyboard().isAccepting())
		newContext = new Editor(_window, std::stoi(_dimensions[0].getText()), std::stoi(_dimensions[1].getText()));
	else if(status.getKeyboard().isPausing())
		newContext = new MainMenu(_window);

	return newContext;
}

void EditorSizeSelection::render()
{
	_window.renderClear();
	for(TextBox& dim : _dimensions)
		dim.render();
	for(TextBox& text : _text)
		text.render();
	_window.renderDraw();
}
