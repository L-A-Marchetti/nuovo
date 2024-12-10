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
	module bd("BASS DRUM", {{"TUNE", 32}, {"LEVEL", 90}, {"ATTACK", 45}, {"DECAY", 100}});
	module sd("SNARE DRUM", {{"TUNE", 20}, {"LEVEL", 85}, {"TONE", 50}, {"SNAPPY", 75}});
	module lt("LOW TOM", {{"TUNE", 40}, {"LEVEL", 80}, {"DECAY", 60}});
	module mt("MID TOM", {{"TUNE", 35}, {"LEVEL", 75}, {"DECAY", 70}});
	module ht("HIGH TOM", {{"TUNE", 45}, {"LEVEL", 90}, {"DECAY", 55}});
	module rshc("RIM SHOT HAND CLAP", {{"LEVEL", 95}, {"LEVEL", 100}});
	module hh("HI HAT", {{"LEVEL", 80}, {"CH DECAY", 65}, {"OH DECAY", 70}});
	module c("CYMBAL", {{"LEVEL", 90}, {"LEVEL", 100}, {"DECAY", 80}, {"CRASH TUNE", 50}, {"RIDE TUNE", 60}});
	this->modules = {&bd, &sd, &lt, &mt, &ht, &rshc, &hh, &c};
	std::cout << "\t" << ++i << ". Modules configured" << std::endl;
	this->r.show(this->modules);
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
