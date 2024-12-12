#ifndef APP_HPP
#define APP_HPP

#define WIN_C SDL_WINDOWPOS_CENTERED

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <thread>
#include <atomic>
#include <chrono>

#include "render.hpp"
#include "controller.hpp"
#include "sequencer.hpp"

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
		sequencer* s;
		bool is_running;
		std::thread bpm_thread;
		void bpm_worker();
	public:
		app(std::string title, int w, int h);
		~app();
		void run();
		void quit();
};

#endif //APP_HPP
