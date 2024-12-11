#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <SDL2/SDL.h>
#include "module.hpp"

typedef struct button
{
	std::string label;
	SDL_Rect r;
	bool state = false;
} button;

class controller
{
	private:
		SDL_Rect screen;
		knob tempo = {"TEMPO", 120};
		knob volume = {"VOLUME", 64};
	public:
		button start = {.label="START"};
		controller();
		~controller();
};

#endif //CONTROLLER_HPP
