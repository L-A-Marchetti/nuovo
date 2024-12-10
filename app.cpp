#include "app.hpp"

app::app(std::string title, int w, int h)
: title(title), w(w), h(h), r(nullptr)
{
	int i = 0;
	std::cout << "App init sequence:" << std::endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cerr << SDL_GetError() << std::endl;
		exit(1);
	}
	else std::cout << "\t" << ++i << ". SDL initialized" << std::endl;
	this->win =
	SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, w, h, 0);
	if (this->win == nullptr)
	{
		std::cerr << SDL_GetError() << std::endl;
		exit(1);
	}
	else std::cout << "\t" << ++i << ". Window created" << std::endl;
	this->r = render(this->win);
	this->is_running = true;
	std::cout << "\t" << ++i << ". App is running" << std::endl;
	return;
}

void app::run()
{
	std::cout << "Main loop started" << std::endl;
	while (this->is_running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) this->is_running = false;
		}
	}
	std::cout << "Main loop finished" << std::endl;
	return;
}

void app::quit()
{
	int i = 0;
	std::cout << "App quit sequence" << std::endl;
	this->r.destroy();
	SDL_DestroyWindow(this->win);
	std::cout << "\t" << ++i << ". Window destroyed" << std::endl;
	SDL_Quit();
	std::cout << "\t" << ++i << ". SDL quit" << std::endl;
	return;
}

app::~app() { return; }
