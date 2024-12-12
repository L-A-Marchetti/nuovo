#ifndef MODULE_HPP
#define MODULE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>

#include "models.hpp"
#include "sequencer.hpp"

class module
{
	private:
		std::string label;
		std::vector<knob> knobs;
		int chan;
	public:
		module(std::string label, std::vector<knob> knobs, std::string file, int chan);
		~module();
		std::string get_label();
		std::vector<knob>& get_knobs();
		Mix_Chunk* sample;
		sequencer* s;
		void play();
};

#endif //MODULE_HPP
