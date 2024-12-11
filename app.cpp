#include "app.hpp"

app::app(std::string title, int w, int h) : title(title), w(w), h(h), r(nullptr)
{
	int i = 0;
	std::cout << "App init sequence:" << std::endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cerr << SDL_GetError() << std::endl;
		exit(1);
	}
	else std::cout << "\t" << ++i << ". SDL initialized" << std::endl;
	this->win = SDL_CreateWindow(title.c_str(), WIN_C, WIN_C, w, h, 0);
	if (this->win == nullptr)
	{
		std::cerr << SDL_GetError() << std::endl;
		exit(1);
	}
	else std::cout << "\t" << ++i << ". Window created" << std::endl;
	this->r = render(this->win);
	this->modules.push_back(new module("BASS DRUM", {{"TUNE", 32}, {"LEVEL", 90}, {"ATTACK", 45}, {"DECAY", 100}}));
	this->modules.push_back(new module("SNARE DRUM", {{"TUNE", 20}, {"LEVEL", 85}, {"TONE", 50}, {"SNAPPY", 75}}));
	this->modules.push_back(new module("LOW TOM", {{"TUNE", 40}, {"LEVEL", 80}, {"DECAY", 60}}));
	this->modules.push_back(new module("MID TOM", {{"TUNE", 35}, {"LEVEL", 75}, {"DECAY", 70}}));
	this->modules.push_back(new module("HIGH TOM", {{"TUNE", 45}, {"LEVEL", 90}, {"DECAY", 55}}));
	this->modules.push_back(new module("RIM SHOT HAND CLAP", {{"LEVEL", 95}, {"LEVEL", 100}}));
	this->modules.push_back(new module("HI HAT", {{"LEVEL", 80}, {"CH DECAY", 65}, {"OH DECAY", 70}}));
	this->modules.push_back(new module("CYMBAL", {{"LEVEL", 90}, {"LEVEL", 100}, {"DECAY", 80}, {"CRASH TUNE", 50}, {"RIDE TUNE", 60}}));
	std::cout << "\t" << ++i << ". Modules configured" << std::endl;
	this->c = new controller();
	this->r.show(this->modules, this->c);
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
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) this->is_running = false;
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
			{
                SDL_Point mouse = {e.button.x, e.button.y};
		if (SDL_PointInRect(&mouse, &this->c->start.r)) this->c->start.state = !this->c->start.state;
                for (size_t m = 0; m < this->modules.size(); ++m)
				{
                    std::vector<knob>& knobs = this->modules[m]->get_knobs();
                    for (size_t i = 0; i < knobs.size(); ++i)
					{
                        if (SDL_PointInRect(&mouse, &knobs[i].fader_rect)) knobs[i].is_dragging = true;
                    }
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
			{
                for (size_t m = 0; m < this->modules.size(); ++m)
				{
                    std::vector<knob>& knobs = this->modules[m]->get_knobs();
                    for (size_t i = 0; i < knobs.size(); ++i) knobs[i].is_dragging = false;
                }
            }
            else if (e.type == SDL_MOUSEMOTION)
			{
    			for (size_t m = 0; m < this->modules.size(); ++m)
				{
    			    std::vector<knob>& knobs = this->modules[m]->get_knobs();
    			    for (size_t i = 0; i < knobs.size(); ++i)
					{
    			        if (knobs[i].is_dragging)
						{
    			            int mouse_x = e.motion.x;
    			            int knob_x = knobs[i].fader_rect.x;
    			            int fader_width = knobs[i].fader_rect.w;
    			            int new_value = std::clamp(mouse_x - knob_x, 0, fader_width);
    			            knobs[i].value = (new_value * 127) / fader_width;
    			        }
    			    }
    			}
			}

        }
		this->r.show(this->modules, this->c);
    }
    std::cout << "Main loop finished" << std::endl;
}


void app::quit()
{
	int i = 0;
	std::cout << "App quit sequence" << std::endl;
	this->r.destroy();
	SDL_DestroyWindow(this->win);
	std::cout << "\t" << ++i << ". Window destroyed" << std::endl;
	// Delete each module to free memory
    	for (module* m : this->modules) {
        delete m;
    	}
    	this->modules.clear();
	delete this->c;
	std::cout << "\t" << ++i << ". Dynamic allocation freed" << std::endl;
	SDL_Quit();
	std::cout << "\t" << ++i << ". SDL quit" << std::endl;
	return;
}

app::~app() { return; }
