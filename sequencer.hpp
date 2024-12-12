#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

#include <vector>

#include "models.hpp"

class sequencer
{
	public:
		std::vector<button> buttons;
		sequencer();
		~sequencer();
};

#endif //SEQUENCER_HPP
