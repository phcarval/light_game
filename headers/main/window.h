#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <stdexcept>
#include <SDL2/SDL.h>

class WindowFactory;
class Rect;
class Texture;

class Window
{
	SDL_Window* _window;
	unsigned int _width;
	unsigned int _height;

	SDL_Renderer* _renderer;

private:
	friend class WindowFactory;
	Window(const std::string& name);

public:
	~Window() {SDL_DestroyRenderer(_renderer); SDL_DestroyWindow(_window);}

	unsigned int getHeight() const {return _height;}
	unsigned int getWidth() const {return _width;}

	SDL_Window* get_SDL_Window() {return _window;}

	SDL_Renderer* get_SDL_Renderer() {return _renderer;}

public:
	void renderSetViewport(const Rect& viewport) const;
	void renderRemoveViewport() const;

	void renderAddTexture(Texture& text, const Rect& size) const;

	void renderDraw() const;
	void renderClear() const;
	
};

class WindowFactory
{
public:
	static Window* createWindow(const std::string& name); //returns NULL if error
};

class WindowException : public std::runtime_error
{
public:
	explicit WindowException(const std::string& error) : std::runtime_error(error) {}
	explicit WindowException(const char* error) : std::runtime_error(error) {}
};


#endif //WINDOW_H