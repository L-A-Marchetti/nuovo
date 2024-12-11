#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

#include "controller.hpp"

class sequencer
{
	public:
		std::vector<button> buttons;
		sequencer();
		~sequencer();
};

#endif //SEQUENCER_HPP
