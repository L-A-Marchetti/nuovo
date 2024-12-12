#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <SDL2/SDL.h>
#include "models.hpp"

class controller
{
	public:
		button start = {.label="START", .size=100, .state=true};
		knob tempo = {"TEMPO", 120};
		knob volume = {"VOLUME", 64};
		controller();
		~controller();
};

#endif //CONTROLLER_HPP
