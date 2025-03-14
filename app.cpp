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
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		std::cerr << Mix_GetError() << std::endl;
		exit(1);
	}
	else std::cout << "\t" << ++i << ". Audio initialized" << std::endl;
	Mix_AllocateChannels(16);
	this->win = SDL_CreateWindow(title.c_str(), WIN_C, WIN_C, w, h, 0);
	if (this->win == nullptr)
	{
		std::cerr << SDL_GetError() << std::endl;
		exit(1);
	}
	else std::cout << "\t" << ++i << ". Window created" << std::endl;
	this->r = render(this->win);
	int chans = 0;
	this->modules.push_back(new module("BASS DRUM", {{"TUNE", 95}, {"LEVEL", 64}, {"ATTACK", 127}, {"DECAY", 127}}, "samples/bd/", ++chans));
	this->modules.push_back(new module("SNARE DRUM", {{"TUNE", 0}, {"LEVEL", 44}, {"TONE", 75}, {"SNAPPY", 45}}, "samples/sd/", ++chans));
	this->modules.push_back(new module("LOW TOM", {{"TUNE", 0}, {"LEVEL", 49}, {"DECAY", 127}}, "samples/lt/", ++chans));
	this->modules.push_back(new module("MID TOM", {{"TUNE", 0}, {"LEVEL", 49}, {"DECAY", 127}}, "samples/mt/", ++chans));
	this->modules.push_back(new module("HIGH TOM", {{"TUNE", 0}, {"LEVEL", 27}, {"DECAY", 127}}, "samples/ht/", ++chans));
	this->modules.push_back(new module("RIM SHOT", {{"LEVEL", 25}}, "samples/rs/", ++chans));
	this->modules.push_back(new module("HAND CLAP", {{"LEVEL", 53}}, "samples/clp/", ++chans));
	this->modules.push_back(new module("CLOSED HAT", {{"LEVEL", 26}, {"DECAY", 39}}, "samples/hhc/", ++chans));
	this->modules.push_back(new module("OPEN HAT", {{"LEVEL", 27}, {"DECAY", 127}}, "samples/hho/", chans));
	this->modules.push_back(new module("CRASH", {{"LEVEL", 25}, {"DECAY", 127}, {"TUNE", 50}}, "samples/crh/", ++chans));
	this->modules.push_back(new module("RIDE", {{"LEVEL", 20}, {"DECAY", 127}, {"TUNE", 50}}, "samples/rd/", ++chans));
	std::cout << "\t" << ++i << ". Modules configured" << std::endl;
	this->c = new controller();
	std::cout << "\t" << ++i << ". Controller created" << std::endl;
	this->bpm_thread = std::thread(&app::bpm_worker, this);
	this->r.show(this->modules, this->c, this->seq_disp);
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
				const Uint8* keystates = SDL_GetKeyboardState(nullptr);
                if (keystates[SDL_SCANCODE_LCTRL] || keystates[SDL_SCANCODE_RCTRL]) {
                    for (int i = 0; i < this->modules[0]->s->buttons.size(); i++)
					{
						if (SDL_PointInRect(&mouse, &this->modules[0]->s->buttons[i].r)) this->seq_disp = i;
					}
					continue;
                }
				if (SDL_PointInRect(&mouse, &this->c->start.r))
				{
					if (this->c->start.state) this->c->start.state = false;
					else 
					{
						if (bpm_thread.joinable()) bpm_thread.join();
						this->c->start.state = true;
						this->bpm_thread = std::thread(&app::bpm_worker, this);
					}
				}
				if (SDL_PointInRect(&mouse, &this->c->tempo.fader_rect)) this->c->tempo.is_dragging = true;
				if (SDL_PointInRect(&mouse, &this->c->volume.fader_rect)) this->c->volume.is_dragging = true;
				for (int i = 0; i < this->modules[seq_disp]->s->buttons.size(); i++)
				{
					if (SDL_PointInRect(&mouse, &this->modules[seq_disp]->s->buttons[i].r)) this->modules[seq_disp]->s->buttons[i].state = !this->modules[seq_disp]->s->buttons[i].state;
				}
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
				this->c->tempo.is_dragging = false;
				this->c->volume.is_dragging = false;
                for (size_t m = 0; m < this->modules.size(); ++m)
				{
                    std::vector<knob>& knobs = this->modules[m]->get_knobs();
                    for (size_t i = 0; i < knobs.size(); ++i) knobs[i].is_dragging = false;
                }
            }
            else if (e.type == SDL_MOUSEMOTION)
			{
				if (this->c->tempo.is_dragging)
				{
					int mouse_x = e.motion.x;
    			    int knob_x = this->c->tempo.fader_rect.x;
    			    int fader_width = this->c->tempo.fader_rect.w;
    			    int new_value = std::clamp(mouse_x - knob_x, 0, fader_width);
    			    this->c->tempo.value = (new_value * 300) / fader_width;
				}
				if (this->c->volume.is_dragging)
				{
					int mouse_x = e.motion.x;
    			    int knob_x = this->c->volume.fader_rect.x;
    			    int fader_width = this->c->volume.fader_rect.w;
    			    int new_value = std::clamp(mouse_x - knob_x, 0, fader_width);
    			    this->c->volume.value = (new_value * 127) / fader_width;
					this->c->set_volume();
				}
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
							if (knobs[i].label == "LEVEL") this->modules[m]->set_chunk_volume();
    			        }
    			    }
    			}
			}

        }
		this->r.show(this->modules, this->c, this->seq_disp);
		SDL_Delay(15);
    }
    std::cout << "Main loop finished" << std::endl;
}

void app::bpm_worker()
{
	int i = 0;
    	using namespace std::chrono;
    	int ticks_per_beat = 4;
		int last_seq_disp = 0;
    	while (this->c->start.state)
		{
			int interval_ms = 60000 / (this->c->tempo.value * ticks_per_beat);
    	    	std::this_thread::sleep_for(milliseconds(interval_ms));
			for (int m = 0; m < this->modules.size(); m++)
			{
				if (this->modules[m]->s->buttons[i%16].state)
				{
					std::thread(&module::play, this->modules[m]).detach();
				}
			}
			this->modules[seq_disp]->s->buttons[i%16].state = !this->modules[seq_disp]->s->buttons[i%16].state;
			if (i != 0) this->modules[last_seq_disp]->s->buttons[(i - 1 + 16) % 16].state = !this->modules[last_seq_disp]->s->buttons[(i - 1 + 16) % 16].state;
			last_seq_disp = seq_disp;
			i++;
    	}
	if (i != 0) this->modules[last_seq_disp]->s->buttons[(i - 1 + 16) % 16].state = !this->modules[last_seq_disp]->s->buttons[(i - 1 + 16) % 16].state;
}

void app::quit()
{
	int i = 0;
	std::cout << "App quit sequence" << std::endl;
	this->c->start.state = false;
        if (bpm_thread.joinable()) bpm_thread.join();
	std::cout << "\t" << ++i << ". Tempo thread joined" << std::endl;
	this->r.destroy();
	for (module* m : this->modules)
	{
    	delete m;
    }
    this->modules.clear();
	std::cout << "\t" << ++i << ". Modules freed" << std::endl;
	Mix_CloseAudio();
	std::cout << "\t" << ++i << ". Audio device closed" << std::endl;
	SDL_DestroyWindow(this->win);
	std::cout << "\t" << ++i << ". Window destroyed" << std::endl;
	// Delete each module to free memory
	delete this->c;
	std::cout << "\t" << ++i << ". Dynamic allocation freed" << std::endl;
	SDL_Quit();
	std::cout << "\t" << ++i << ". SDL quit" << std::endl;
	return;
}

app::~app() { return; }
