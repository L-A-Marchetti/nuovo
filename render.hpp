#ifndef RENDER_HPP
#define RENDER_HPP


#define TR_FONT "fonts/TR-909.ttf"
#define BASIC "fonts/coolvetica.ttf"
#define SCREEN "fonts/7segment.ttf"
#define BLUE_GRAY (SDL_Color){46, 67, 71, 255}
#define GRAY (SDL_Color){196, 200, 194, 255}
#define DARK_ORANGE (SDL_Color){106, 186, 230, 255}
#define DARK_GRAY (SDL_Color){90, 99, 107, 255}
#define B_LIGHT (SDL_Color){237, 226, 195, 255}
#define B_SHADOW (SDL_Color){184, 174, 145, 255}
#define LED (SDL_Color){127, 255, 255, 255} // 127, 255, 255
#define SCREEN_BG (SDL_Color){2, 189, 189, 255} // 3, 255, 255

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "text.hpp"
#include "module.hpp"
#include "controller.hpp"
#include "sequencer.hpp"

class render
{
	private:
		SDL_Renderer* r;
	public:
		render(SDL_Window* win);
		~render();
		void background();
		void modules(const std::vector<module*>& modules);
		void control(controller* c, std::vector<module*> modules);
		void but(const std::vector<button*>& b, int x, int y, int pad);
		void seq(sequencer* s);
		void show(const std::vector<module*>& modules, controller* c, int seq_disp);
		SDL_Renderer* get_renderer();
		void destroy();
};

#endif //RENDER_HPP
