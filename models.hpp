#ifndef MODELS_HPP
#define MODELS_HPP

#include <SDL2/SDL.h>
#include <string>

typedef struct knob
{
	std::string label;
	int value;
	SDL_Rect fader_rect;
    SDL_Rect cursor_rect;
	bool is_dragging = false;
} knob;

typedef struct button
{
	std::string label;
	int size;
	SDL_Rect r;
	bool state = false;
} button;

#endif //MODELS_HPP