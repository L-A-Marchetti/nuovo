#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "models.hpp"

class controller
{
	public:
		button start = {.label="START", .size=100, .state=true};
		knob tempo = {"TEMPO", 129};
		knob volume = {"VOLUME", 64};
		controller();
		~controller();
		void set_volume();
};

#endif //CONTROLLER_HPP
