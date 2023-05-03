#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include "graphics/graphical.h"
#include "main/common.h"
//#include "interface/shape.h"

#include <array>
#include <iostream>

class InputStatus;
class Rect;

class Interactive : public Graphical //interface to create interactive objects on screen
{
private:
	bool _is_hovered;

	MouseBool _is_dragged;
	MouseBool _is_clicked;
	MouseBool _is_selected;
	MouseBool _stoppedDragging;

	Graphical* _graphicsForDragging;

protected:
	bool _to_be_destroyed;
	bool _can_be_dragged;
	
public:
	Interactive(Window& window, const std::string& textPath, const Rect& rect, bool drag) :
												Graphical(window, textPath, rect),
												_graphicsForDragging(nullptr),
												_can_be_dragged(drag),
												_is_dragged({false}),
												_is_selected({false}),
												_is_hovered(false),
												_to_be_destroyed(false),
												_is_clicked({false}),
												_stoppedDragging({false}) {}

	Interactive(Window& window, const Rect& rect, bool drag) :
												Graphical(window, rect),
												_graphicsForDragging(nullptr),
												_can_be_dragged(drag),
												_is_dragged({false}),
												_is_selected({false}),
												_is_hovered(false),
												_to_be_destroyed(false),
												_is_clicked({false}),
												_stoppedDragging({false}) {}

	/*Interactive(Window& window, const Rect& rect, const std::string& textPath = "") : Graphical(window, textPath, rect),
																				 _is_dragged({false}),
																				 _is_clicked({false}),
																				 _is_hovered(false),
																				 _is_selected({false}) {}
*/
    /*Interactive(const Interactive& other);
    Interactive(Interactive&& other);*/

    Interactive& operator=(const Interactive& other);
	Interactive& operator=(Interactive&& other);

	virtual ~Interactive() {if(_graphicsForDragging != nullptr) delete _graphicsForDragging; _graphicsForDragging = NULL;}

public:
	/*
	 * updates the internal state of the object
	 */
	void updateEvents(const InputStatus& status);

public:
	bool isHovering() const {return _is_hovered;}
	bool isDragging(M_BUTTON but) const {return _is_dragged[but];}
	bool isClicking(M_BUTTON but) const {return _is_clicked[but];}
	bool stoppedDragging(M_BUTTON but) const {return _stoppedDragging[but];}

	bool isDraggable() const {return _can_be_dragged;}

public:
	bool toBeDestroyed() const {return _to_be_destroyed;}
	void resetDestruction() {_to_be_destroyed = false;}

	Interactive(const Interactive& other) : Graphical(other),
											_graphicsForDragging(nullptr),
											_can_be_dragged(other._can_be_dragged),
											_is_dragged(other._is_dragged),
											_is_selected(other._is_selected),
											_is_hovered(other._is_hovered),
											_to_be_destroyed(other._to_be_destroyed),
											_is_clicked(other._is_clicked),
											_stoppedDragging(other._stoppedDragging) {if(other._graphicsForDragging != NULL) this->_graphicsForDragging = new Graphical(*other._graphicsForDragging);}
											
	Interactive(Interactive&& other) : Graphical(other),
									   _graphicsForDragging(other._graphicsForDragging),
									   _can_be_dragged(other._can_be_dragged),
									   _is_dragged(other._is_dragged),
									   _is_selected(other._is_selected),
									   _is_hovered(other._is_hovered),
									   _to_be_destroyed(other._to_be_destroyed),
									   _is_clicked(other._is_clicked),
									   _stoppedDragging(other._stoppedDragging) {other._graphicsForDragging = NULL;}

private:
	void startHovering() {_is_hovered = true;}
	void startDragging(M_BUTTON but) {_is_dragged[but] = true;}
	void startClicking(M_BUTTON but) {_is_clicked[but] = true;}
	void startSelecting(M_BUTTON but) {_is_selected[but] = true;}

	void stopHovering() {_is_hovered = false;}
	void stopDragging(M_BUTTON but) {_is_dragged[but] = false; _stoppedDragging[but] = true;}
	void stopClicking(M_BUTTON but) {_is_clicked[but] = false;}
	void stopSelecting(M_BUTTON but) {_is_selected[but] = false;}
/*
protected:
	virtual void onDragStop(const InputStatus& status) {}
	virtual void onHover(const InputStatus& status) {}
	virtual void onClick(const InputStatus& status, M_BUTTON but) {}*/

private:
	void dragCheck(M_BUTTON but, const InputStatus& status);
	void checkClickEvents(M_BUTTON but, const InputStatus& status);

private:
	virtual void manageKeyboard(const InputStatus& status) {}

public:
	void render() const override;
};

#endif //INTERACTIVE_H