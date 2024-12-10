#ifndef RENDER_HPP
#define RENDER_HPP


#define TR_FONT "TR-909.ttf"
#define BASIC "coolvetica.ttf"
#define BLUE_GRAY (SDL_Color){46, 67, 71, 255}
#define GRAY (SDL_Color){196, 200, 194, 255}
#define DARK_ORANGE (SDL_Color){239, 125, 58, 255}
#define DARK_GRAY (SDL_Color){90, 99, 107, 255}

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "text.hpp"
#include "module.hpp"

class render
{
	private:
		SDL_Renderer* r;
	public:
		render(SDL_Window* win);
		~render();
		void background();
		void modules(const std::vector<module*>& modules);
		void show();
		SDL_Renderer* get_renderer();
		void destroy();
};

#endif //RENDER_HPP
