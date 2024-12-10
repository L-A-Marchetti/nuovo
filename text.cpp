#include "text.hpp"

text::text(SDL_Renderer* r, int fs, std::string fn) : r(r)
{
	this->r = r;
	this->f = TTF_OpenFont(fn.c_str(), fs);
        if (this->f == nullptr) std::cerr << TTF_GetError() << std::endl;
	return;
}

void text::write(std::string t, SDL_Color c, int x, int y)
{
	SDL_Surface* s = TTF_RenderText_Blended(this->f, t.c_str(), c);
        if (s == nullptr) std::cerr << TTF_GetError() << std::endl;
        SDL_Texture* tx = SDL_CreateTextureFromSurface(this->r, s);
        if (tx == nullptr) std:: cerr << TTF_GetError() << std::endl;
        SDL_Rect r = {x, y, s->w, s->h};
        if (SDL_RenderCopy(this->r, tx, nullptr, &r) < 0)std::cerr << SDL_GetError() << std::endl;
	SDL_DestroyTexture(tx);
	SDL_FreeSurface(s);
	return;
}

void text::destroy()
{
	int i = 0;
    TTF_CloseFont(this->f);
	return;
}

text::~text() { return; }
