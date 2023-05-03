#include "command_input/input_status.h"
#include "graphics/interactive_text_box.h"

void InteractiveTextBox::manageKeyboard(const InputStatus& status)
{
	const InputStatus::Keyboard& kd = status.getKeyboard();

	if(kd.isAccepting())
		_to_be_destroyed = true;

	if(kd.isBackspacing())
	{
		if(!_text.empty())
			_text.pop_back();
	}

	if(kd.isPausing())
	{
		_text = "";
		_to_be_destroyed = true;
	}

	_text += kd.getTextInput();

	this->updateText(_text);
}
