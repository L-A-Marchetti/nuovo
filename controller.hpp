#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <SDL2/SDL.h>
#include "module.hpp"

typedef struct button
{
	std::string label;
	int size;
	SDL_Rect r;
	bool state = false;
} button;

class controller
{
	public:
		button start = {.label="START", .size=100};
		knob tempo = {"TEMPO", 120};
		knob volume = {"VOLUME", 64};
		controller();
		~controller();
};

#endif //CONTROLLER_HPP
