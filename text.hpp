#ifndef TEXT_HPP
#define TEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

class text
{
	private:
		SDL_Renderer* r;
                TTF_Font* f;
	public:
		text(SDL_Renderer* r, int fs, std::string fn);
		void write(std::string t, SDL_Color c, int x, int y);
		void destroy();
		~text();
};

#endif //TEXT_HPP
