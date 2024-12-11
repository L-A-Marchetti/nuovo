#ifndef MODULE_HPP
#define MODULE_HPP

#include <SDL2/SDL.h>
#include <string>
#include <vector>

typedef struct knob
{
	std::string label;
	int value;
	SDL_Rect fader_rect;
    	SDL_Rect cursor_rect;
	bool is_dragging = false;
} knob;

class module
{
	private:
		std::string label;
		std::vector<knob> knobs;
	public:
		module(std::string label, std::vector<knob> knobs);
		~module();
		std::string get_label();
		std::vector<knob>& get_knobs();
};

#endif //MODULE_HPP
