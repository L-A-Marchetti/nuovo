#ifndef APP_HPP
#define APP_HPP

#define WIN_C SDL_WINDOWPOS_CENTERED

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <algorithm>

#include "render.hpp"
#include "controller.hpp"

class app
{
	private:
		std::string title;
		int w;
		int h;
		SDL_Window* win;
		render r;
		std::vector<module*> modules;
		controller* c;
		bool is_running;
	public:
		app(std::string title, int w, int h);
		~app();
		void run();
		void quit();
};

#endif //APP_HPP
