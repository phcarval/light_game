#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "SDL2/SDL.h"

class Window;
class Rect;

class Texture //Wrapper class for manipulating SDL_Surface
{
private:
	SDL_Texture* _texture;
	std::string _path;
	Window& _window;

	std::string _text;
	SDL_Texture* _textureFromText;

	std::string _counter;
	SDL_Texture* _textureFromCounter;

	SDL_Color _modColor;

public:
	Texture(Window& window, const std::string& path, const std::string& cnt = "", const std::string& text = "");
	
	Texture(Window& window) : _texture(NULL), _textureFromText(NULL), _textureFromCounter(NULL), _text(""), _path(""), _counter(""), _window(window), _modColor{255,255,255,0} {}
	Texture(const Texture& other) : _texture(NULL),
									_textureFromText(NULL),
									_textureFromCounter(NULL),
									_window(other._window),
									_text(other._text),
									_path(other._path),
									_counter(other._counter),
									_modColor(other._modColor) {all_init();}

	~Texture() {if(_texture != NULL) SDL_DestroyTexture(_texture); if(_textureFromText != NULL) SDL_DestroyTexture(_textureFromText); if(_textureFromCounter != NULL) SDL_DestroyTexture(_textureFromCounter);}

	Texture& operator=(const Texture& other);
	Texture& operator=(Texture&& other);

	Texture(Texture&& other) : _texture(other._texture),
							   _window(other._window),
							   _path(other._path),
							   _text(other._text),
							   _textureFromText(other._textureFromText),
							   _textureFromCounter(other._textureFromCounter),
							   _counter(other._counter),
							   _modColor(other._modColor)
							   {other._texture = NULL; other._textureFromText = NULL; other._textureFromCounter = NULL;}

	void updateText(const std::string& text);
	void updateTexture(const std::string& path);
	void updateCounter(const std::string& counter);

	void setColor(uint8_t red, uint8_t green, uint8_t blue);

private:
	friend class Window;
	SDL_Texture* getTexture() {return _texture;} // MAY RETURN NULL!
	const SDL_Texture* getTexture() const {return _texture;}

	SDL_Texture* getTextTexture() {return _textureFromText;}
	const SDL_Texture* getTextTexture() const {return _textureFromText;}

	SDL_Texture* getCounterTexture() {return _textureFromCounter;}
	const SDL_Texture* getCounterTexture() const {return _textureFromCounter;}

private:
	void all_init();

	void main_texture_init();
	void text_texture_init();
	void counter_texture_init();

public:
	void render(const Rect& position);
};

#endif //TEXTURE_H