#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "graphics/texture.h"
#include "main/window.h"

#include <iostream>

Texture::Texture(Window& window, const std::string& path, const std::string& cnt, const std::string& text) : _texture(NULL),
																					 _textureFromText(NULL),
																					 _textureFromCounter(NULL),
																					 _counter(cnt),
																					 _window(window),
																					 _path(path),
																					 _text(text),
																					 _modColor{255,255,255,0}
{
	all_init();
}

void Texture::all_init()
{
	main_texture_init();
	text_texture_init();
	counter_texture_init();
}

Texture& Texture::operator=(const Texture& other)
{
	this->_path = other._path;
	this->_window = other._window;
	this->_text = other._text;
	this->_counter = other._counter;
	this->_modColor = other._modColor;

	SDL_DestroyTexture(this->_texture);
	SDL_DestroyTexture(this->_textureFromText);
	SDL_DestroyTexture(this->_textureFromCounter);

	this->_texture = NULL;
	this->_textureFromText = NULL;
	this->_textureFromCounter = NULL;

	all_init();
	return *this;
}

void Texture::main_texture_init()
{
	if(_texture != NULL)
	{
		SDL_DestroyTexture(_texture);
		_texture = NULL;
	}
	if(_path != "")
	{
		SDL_Surface* surf = NULL;

		surf = IMG_Load(_path.c_str());
		if(surf == NULL)
		{
			std::cout << "Couldn't load texture : " << _path << IMG_GetError() << std::endl;
		}
		else
		{
			_texture = SDL_CreateTextureFromSurface(_window.get_SDL_Renderer(), surf);
			SDL_FreeSurface(surf);
		}
	}
}

void Texture::text_texture_init()
{
	if(_textureFromText != NULL)
	{
		SDL_DestroyTexture(_textureFromText);
		_textureFromText = NULL;
	}
	if(!_text.empty())
	{
		TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf",20);
		if(font != NULL)
		{
			SDL_Color color = {255,0,0};
			SDL_Surface* textSurface = TTF_RenderText_Blended(font, _text.c_str(), color);
			TTF_CloseFont(font);
			if(textSurface == NULL)
			{
				std::cout << "Error : " << TTF_GetError() << std::endl;
			}
			else
			{
				_textureFromText = SDL_CreateTextureFromSurface(_window.get_SDL_Renderer(), textSurface);
				SDL_FreeSurface(textSurface);
			}
		}
		else
		{
			std::cout << "font loading failed : " << TTF_GetError() << std::endl;
		}
	}
}

void Texture::counter_texture_init()
{
	if(_textureFromCounter != NULL)
	{
		SDL_DestroyTexture(_textureFromCounter);
		_textureFromCounter = NULL;
	}
	if(!_counter.empty())
	{
		TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf",20);
		if(font != NULL)
		{
			SDL_Color color = {255,0,0};
			SDL_Surface* textSurface = TTF_RenderText_Blended(font, _counter.c_str(), color);
			TTF_CloseFont(font);
			if(textSurface == NULL)
			{
				std::cout << "Error : " << TTF_GetError() << std::endl;
			}
			else
			{
				_textureFromCounter = SDL_CreateTextureFromSurface(_window.get_SDL_Renderer(), textSurface);
				SDL_FreeSurface(textSurface);
			}
		}
		else
		{
			std::cout << "font loading failed : " << TTF_GetError() << std::endl;
		}
	}
}

Texture& Texture::operator=(Texture&& other)
{
	this->_path = other._path;
	this->_window = other._window;
	this->_texture = other._texture;

	this->_text = other._text;
	this->_textureFromText = other._textureFromText;

	this->_counter = other._counter;
	this->_textureFromCounter = other._textureFromCounter;

	this->_modColor = other._modColor;

	other._texture = NULL;
	other._textureFromText = NULL;

	return *this;
}

void Texture::updateText(const std::string& text)
{
	if(_text!=text)
	{
		_text = text;
		text_texture_init();
	}
}

void Texture::updateTexture(const std::string& path)
{
	if(_path!=path)
	{
		_path = path;
		main_texture_init();
	}
}

void Texture::updateCounter(const std::string& counter)
{
	if(counter != _counter)
	{
		_counter = counter;
		counter_texture_init();
	}
}

void Texture::render(const Rect& position)
{
	SDL_SetTextureColorMod(_texture, _modColor.r, _modColor.g, _modColor.b);
	_window.renderAddTexture(*this, position);
}

void Texture::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
	_modColor = {red,green,blue,0};
}
