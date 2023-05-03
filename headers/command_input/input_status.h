#ifndef INPUT_STATUS_H
#define INPUT_STATUS_H

#include <set>
#include <chrono>
#include <array>

#include "main/common.h"
#include "SDL2/SDL.h"

#define CLICK_DURATION 100
#define DISTANCE_DRAG_LIMIT 5.0

typedef std::chrono::high_resolution_clock hi_res_clock;
typedef std::array<hi_res_clock::time_point,N_MOUSE_BUTTONS> TimeBool;

class InputStatus
{
public:
	class Mouse
	{
	private:
		int _clickDurationBeforeDrag;
		int _distanceBeforeDrag;

		MouseBool _isPressing;
		MouseBool _isClicking;
		MouseBool _isHolding;
		MouseBool _isScrolling;

		ClickBool _clickPosition;

		TimeBool _clickTime;

		Position _position;

	private:
		friend class InputStatus;

		void initRoutine();

		void startPressing(M_BUTTON button) {_isPressing[button] = true; _clickPosition[button] = _position;}
		void stopPressing(M_BUTTON button) {_isPressing[button] = false; _clickPosition[button] = Position();}

		void startCountingTime(M_BUTTON button) {_clickTime[button] = hi_res_clock::now();}

		void startClicking(M_BUTTON button) {_isClicking[button] = true;}
		void stopClicking() {_isClicking = {false,false};}

		void startHolding(M_BUTTON button) {_isHolding[button] = true;}
		void stopHolding(M_BUTTON button) {_isHolding[button] = false;}

		void scroll(M_BUTTON direction) {_isScrolling[direction] = true;}

		void stopScrolling() {_isScrolling = {false,false};}

		void setPosition(const Position& pos) {_position = pos;}

		Mouse() : _isPressing({false,false}),
				  _isClicking({false,false}),
				  _isHolding({false,false}),
				  _isScrolling({false,false}),
				  _clickDurationBeforeDrag(CLICK_DURATION),
				  _distanceBeforeDrag(DISTANCE_DRAG_LIMIT),
				  _position(Position()) {}

	private:
		void countTimePassed(M_BUTTON button);
		void measureDistance(M_BUTTON button);

	public:
		/* will be true as long as the mouse button is pressed but it is not considered a click or a drag */
		bool isPressing(M_BUTTON button) const {return _isPressing[button];}

		/* will be true for one cycle once the mouse button is released clickly (before it starts holding) */
		bool isClicking(M_BUTTON button) const {return _isClicking[button];}

		/*
		 *will be true as long as the mouse button is held longer than CLICK_DURATION ms or
		 *the mouse pointer has moved DISTANCE_DRAG_LIMIT away from the initial click position
		 */
		bool isHolding(M_BUTTON button) const {return _isHolding[button];}

		/* will be true for one cycle */
		bool isScrolling(M_BUTTON direction) const {return _isScrolling[direction];}
		
		const Position& getPosition() const {return _position;}
		Position& getPosition() {return _position;}
	};
	class Keyboard
	{
	private:
		std::set<SDL_Keycode> _pressedKeys;
		std::set<SDL_Keycode> _heldKeys;

		std::string _writtenText;

	private:
		friend class InputStatus;
		void initRoutine();

		void updateTextInput(const std::string& str) {_writtenText += str;}
		void resetTextInput() {_writtenText = "";}

		void unpressButtons() {_pressedKeys = std::set<SDL_Keycode>();}

		void addKey(const SDL_Keycode& key) {_pressedKeys.insert(key); _heldKeys.insert(key);}
		void removeKey(const SDL_Keycode& key) {if(_heldKeys.count(key)) _heldKeys.erase(key);}

	public: // to be configured easily
		bool isPausing() const {return _pressedKeys.count(SDLK_ESCAPE);}
		bool isRotating() const {return _pressedKeys.count(SDLK_r);}
		bool isUp() const {return _pressedKeys.count(SDLK_UP);}
		bool isDown() const {return _pressedKeys.count(SDLK_DOWN);}
		bool isLeft() const {return _pressedKeys.count(SDLK_LEFT);}
		bool isRight() const {return _pressedKeys.count(SDLK_RIGHT);}
		bool isSaving() const {return _pressedKeys.count(SDLK_s);}
		bool isAccepting() const {return _pressedKeys.count(SDLK_RETURN);}
		bool isBackspacing() const {return _pressedKeys.count(SDLK_BACKSPACE);}
		bool isTogglingMovable() const {return _pressedKeys.count(SDLK_m);}

		bool isPressingCTRL() const {return SDL_GetModState() & KMOD_CTRL;}

		const std::string& getTextInput() const {return _writtenText;}
	};
private:
	Mouse _mouse;
	Keyboard _keyboard;

	bool _quit;

public:
	void updateEvents();

	const Mouse& getMouse() const {return _mouse;}
	Mouse& getMouse() {return _mouse;}

	const Keyboard& getKeyboard() const {return _keyboard;}
	Keyboard& getKeyboard() {return _keyboard;}

	bool isQuitting() const {return _quit;}

	InputStatus() : _mouse(Mouse()), _keyboard(Keyboard()), _quit(false) {}
};

#endif //INPUT_STATUS_H