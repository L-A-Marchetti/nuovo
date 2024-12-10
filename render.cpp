#include "render.hpp"

render::render(SDL_Window* win)
{
	if (win == nullptr) return;
	int i = 0;
	std::cout << "\tRender init sequence" << std::endl;
	this->r = SDL_CreateRenderer(win, -1, 0);
        if (r == nullptr)
        {
                std::cerr << SDL_GetError() << std::endl;
                exit(1);
        }
        else std::cout << "\t\t" << ++i << ". Renderer created" << std::endl;
	if (TTF_Init() < 0)
        {
                std::cerr << TTF_GetError() << std::endl;
                exit(1);
        }
        else std::cout << "\t\t" << ++i << ". TTF initialized" << std::endl;
	return;
}

render::~render() { return; };

void render::background()
{
	SDL_SetRenderDrawColor(this->r, GRAY.r, GRAY.g, GRAY.b, GRAY.a);
	SDL_RenderClear(this->r);
	return;
}

void render::modules(const std::vector<module*>& modules)
{
    int x_start = 20; // Start x pos
    int y = 150;      // Start y pos
    int h = 35;       // Labels height
    int w = 186;      // Modules width
    int module_spacing = 10; // Modules x padding
    text label(this->r, 18, BASIC);
    text knob_label(this->r, 12, BASIC);
    for (size_t m_index = 0; m_index < modules.size(); m_index++)
    {
        module* m = modules[m_index];
        int x = x_start + m_index * (w + module_spacing); // Calculated x pos
        // Label Background
        SDL_Rect label_bg = {x, y, w, h};
        SDL_SetRenderDrawColor(this->r, BLUE_GRAY.r, BLUE_GRAY.g, BLUE_GRAY.b, BLUE_GRAY.a);
        SDL_RenderFillRect(this->r, &label_bg);
        // Label Text
        int label_text_width = size(m->get_label()) * 18 * 0.5f; // Text width approximation
        int label_center_x = x + (w / 2) - (label_text_width / 2); // Center on x
        label.write(m->get_label(), DARK_ORANGE, label_center_x, (y + h / 6) + 1); // Center on y
        // Faders configuration
        int fader_height = 10;
        int light_height = fader_height / 3;
        int cursor_width = 10;
        int cursor_height = 20;
        int y_padding = 30; // Padding between faders
        // Faders
        std::vector<knob> knobs = m->get_knobs();
        for (size_t i = 0; i < knobs.size(); i++)
        {
            int y_position = y + h + (i + 1) * y_padding + i * (fader_height + cursor_height);
            // Fader background
            SDL_Rect fader_bg = {x, y_position, w, fader_height};
            SDL_SetRenderDrawColor(this->r, DARK_GRAY.r, DARK_GRAY.g, DARK_GRAY.b, DARK_GRAY.a);
            SDL_RenderFillRect(this->r, &fader_bg);
            float normalized_value = knobs[i].value / 127.0f;
            // Fader light
            SDL_Rect fader_light = {x + 2, y_position + 3, static_cast<int>(normalized_value * (w - 4)), light_height};
            SDL_SetRenderDrawColor(this->r, DARK_ORANGE.r, DARK_ORANGE.g, DARK_ORANGE.b, DARK_ORANGE.a);
            SDL_RenderFillRect(this->r, &fader_light);
            // Fader cursor
            SDL_Rect fader_cursor = {x + static_cast<int>(normalized_value * w) - cursor_width / 2, y_position - 5, cursor_width, cursor_height};
            SDL_SetRenderDrawColor(this->r, BLUE_GRAY.r, BLUE_GRAY.g, BLUE_GRAY.b, BLUE_GRAY.a);
            SDL_RenderFillRect(this->r, &fader_cursor);
            // Knob label
            int knob_text_width = size(knobs[i].label) * 12 * 0.6f; // Facteur ajusté pour la largeur du texte
            int knob_center_x = x + (w / 2) - (knob_text_width / 2); // Centrage horizontal
            knob_label.write(knobs[i].label, BLUE_GRAY, knob_center_x, y_position + cursor_height); // Position verticale ajustée
        }
    }
    knob_label.destroy();
    label.destroy();
}

void render::show(const std::vector<module*>& modules)
{
        this->background();
        text model(this->r, 76, TR_FONT);
        model.write("NUOVO", BLUE_GRAY, 20, 50);
        text slogan(this->r, 36, TR_FONT);
        slogan.write("Rhythm Composer", BLUE_GRAY, 1150, 70);
        this->modules(modules);
	model.destroy();
	slogan.destroy();
	SDL_RenderPresent(this->r);
	return;
}

SDL_Renderer* render::get_renderer() { return this->r; }

void render::destroy()
{
	int i = 0;
	std::cout << "\tRender destroy sequence" << std::endl;
	TTF_Quit();
        std::cout << "\t\t" << ++i << ". TTF quit" << std::endl;
        SDL_DestroyRenderer(this->r);
        std::cout << "\t\t" << ++i << ". Renderer destroyed" << std::endl;
	return;
}
