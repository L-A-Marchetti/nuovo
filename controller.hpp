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
		knob volume = {"VOLUME", 64};
	public:
		button start = {.label="START"};
		knob tempo = {"TEMPO", 120};
		controller();
		~controller();
};

#endif //CONTROLLER_HPP
