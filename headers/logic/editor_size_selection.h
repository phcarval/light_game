#ifndef EDITOR_SIZE_SELECTION
#define EDITOR_SIZE_SELECTION

#include <array>

#include "main/menu.h"
#include "graphics/text_box.h"

class InputStatus;

class EditorSizeSelection : public Menu
{
private:
	std::array<TextBox,2> _dimensions;
	std::array<TextBox,2> _text;

public:
	EditorSizeSelection(Window& window) : Menu(window), _dimensions{
														TextBox(window, "5", Rect(900,300,100,100)),
														TextBox(window, "5", Rect(1550,300,100,100))},
														_text{
														TextBox(window, "Height:", Rect(300, 300, 600, 100)),
														TextBox(window, "Width:", Rect(1050, 300, 500, 100))} {}

	WindowContext* updateContext(const InputStatus& status) override;

	void render() override;

private:
	void init();
};

#endif //EDITOR_SIZE_SELECTION