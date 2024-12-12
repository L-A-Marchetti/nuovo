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
    int jumped = 0;
    text label(this->r, 18, BASIC);
    text knob_label(this->r, 12, BASIC);
    for (size_t m_index = 0; m_index < modules.size(); m_index++)
    {
        int x = 0;
        module* m = modules[m_index];
        if (m->get_label() == "HAND CLAP" || m->get_label() == "CLOSED HAT" || m->get_label() == "RIDE")
        {
                x = x_start + (m_index-(m->get_label() == "RIDE" ? 3 : 1)) * (w + module_spacing);
                y = m->get_label() == "HAND CLAP" ? 270 : m->get_label() == "RIDE" ? 390 : 330;
                jumped++;
        }
        else
        {
                x = x_start + (m_index-jumped) * (w + module_spacing); // Calculated x pos
                y = 150;
        }
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
        std::vector<knob>& knobs = m->get_knobs();
        for (size_t i = 0; i < knobs.size(); i++)
        {
            int y_position = y + h + (i + 1) * y_padding + i * (fader_height + cursor_height);
            // Fader background
            knobs[i].fader_rect = {x, y_position, w, fader_height};
            SDL_SetRenderDrawColor(this->r, DARK_GRAY.r, DARK_GRAY.g, DARK_GRAY.b, 255);
            SDL_RenderFillRect(this->r, &knobs[i].fader_rect);
            float normalized_value = knobs[i].value / 127.0f;
            // Fader light
            SDL_Rect fader_light = {x + 2, y_position + 3,
	    static_cast<int>(normalized_value * (w - 4)), light_height};
            SDL_SetRenderDrawColor(this->r, DARK_ORANGE.r, DARK_ORANGE.g, DARK_ORANGE.b, 255);
            SDL_RenderFillRect(this->r, &fader_light);
            // Fader cursor
            knobs[i].cursor_rect = {x + static_cast<int>(normalized_value * w)
	    - cursor_width / 2, y_position - 5, cursor_width, cursor_height};
            SDL_SetRenderDrawColor(this->r, BLUE_GRAY.r, BLUE_GRAY.g, BLUE_GRAY.b, 255);
            SDL_RenderFillRect(this->r, &knobs[i].cursor_rect);
            // Knob label
            int knob_text_width = size(knobs[i].label) * 12 * 0.6f; // Facteur ajusté pour la largeur du texte
            int knob_center_x = x + (w / 2) - (knob_text_width / 2); // Centrage horizontal
            knob_label.write(knobs[i].label, DARK_GRAY, knob_center_x,
	    y_position + cursor_height); // Position verticale ajustée
        }
    }
    knob_label.destroy();
    label.destroy();
}

void render::control(controller* c)
{
	int x = 20;
	int y = 520;
	int pad = 60;
	text label(this->r, 18, BASIC);
	text screen(this->r, 76, SCREEN);
        // Button
        std::vector<button*> b = {&c->start};
        this->but(b, x, y, pad);
	// Tempo Label
	int label_text_width = c->tempo.label.size() * 18 * 0.5f;
        int label_center_x = c->start.r.x + c->start.r.w + pad + (200 / 2) - (label_text_width / 2); // Center on x
        label.write(c->tempo.label, DARK_GRAY, label_center_x, y);
	// Screen background
        SDL_Rect screen_bg = {c->start.r.x + c->start.r.w + pad, c->start.r.y + 15, 200, c->start.size-30};
        SDL_SetRenderDrawColor(this->r, SCREEN_BG.r, SCREEN_BG.g, SCREEN_BG.b, SCREEN_BG.a);
        SDL_RenderFillRect(this->r, &screen_bg);
        // Screen Text
        int screen_text_width = std::to_string(c->tempo.value).size() * 76 * .52f; // Text width approximation
        int screen_center_x = screen_bg.x + (screen_bg.w / 2) - (screen_text_width / 2); // Center on x
        screen.write(std::to_string(c->tempo.value), LED, screen_center_x, (screen_bg.y - 2));
        // faders
        // Fader background
        int w = 300;
        int fader_height = 10;
        int light_height = fader_height / 3;
        int cursor_width = 10;
        int cursor_height = 20;
        int y_pad = 60;
        text knob_label(this->r, 12, BASIC);
        //TEMPO
        c->tempo.fader_rect = {screen_bg.x + screen_bg.w + pad, screen_bg.y, w, fader_height};
        SDL_SetRenderDrawColor(this->r, DARK_GRAY.r, DARK_GRAY.g, DARK_GRAY.b, 255);
        SDL_RenderFillRect(this->r, &c->tempo.fader_rect);
        float normalized_value = c->tempo.value / 300.0f;
        // Fader light
        SDL_Rect fader_light = {screen_bg.x + screen_bg.w + pad + 2, screen_bg.y + 3,
	static_cast<int>(normalized_value * (w - 4)), light_height};
        SDL_SetRenderDrawColor(this->r, DARK_ORANGE.r, DARK_ORANGE.g, DARK_ORANGE.b, 255);
        SDL_RenderFillRect(this->r, &fader_light);
        // Fader cursor
        c->tempo.cursor_rect = {screen_bg.x + screen_bg.w + pad + static_cast<int>(normalized_value * w)
	- cursor_width / 2, screen_bg.y - 5, cursor_width, cursor_height};
        SDL_SetRenderDrawColor(this->r, BLUE_GRAY.r, BLUE_GRAY.g, BLUE_GRAY.b, 255);
        SDL_RenderFillRect(this->r, &c->tempo.cursor_rect);
        // Knob label
        int knob_text_width = c->tempo.label.size() * 12 * 0.6f; // Facteur ajusté pour la largeur du texte
        int knob_center_x = screen_bg.x + screen_bg.w + pad + (w / 2) - (knob_text_width / 2); // Centrage horizontal
        knob_label.write(c->tempo.label, DARK_GRAY, knob_center_x,
	screen_bg.y + cursor_height); // Position verticale ajustée
        //VOLUME
        c->volume.fader_rect = {screen_bg.x + screen_bg.w + pad, screen_bg.y + y_pad, w, fader_height};
        SDL_SetRenderDrawColor(this->r, DARK_GRAY.r, DARK_GRAY.g, DARK_GRAY.b, 255);
        SDL_RenderFillRect(this->r, &c->volume.fader_rect);
        normalized_value = c->volume.value / 127.0f;
        // Fader light
        fader_light = {screen_bg.x + screen_bg.w + pad + 2, screen_bg.y + y_pad + 3,
	static_cast<int>(normalized_value * (w - 4)), light_height};
        SDL_SetRenderDrawColor(this->r, DARK_ORANGE.r, DARK_ORANGE.g, DARK_ORANGE.b, 255);
        SDL_RenderFillRect(this->r, &fader_light);
        // Fader cursor
        c->volume.cursor_rect = {screen_bg.x + screen_bg.w + pad + static_cast<int>(normalized_value * w)
	- cursor_width / 2, screen_bg.y + y_pad - 5, cursor_width, cursor_height};
        SDL_SetRenderDrawColor(this->r, BLUE_GRAY.r, BLUE_GRAY.g, BLUE_GRAY.b, 255);
        SDL_RenderFillRect(this->r, &c->volume.cursor_rect);
        // Knob label
        knob_text_width = c->volume.label.size() * 12 * 0.6f; // Facteur ajusté pour la largeur du texte
        knob_center_x = screen_bg.x + screen_bg.w + pad + (w / 2) - (knob_text_width / 2); // Centrage horizontal
        knob_label.write(c->volume.label, DARK_GRAY, knob_center_x,
	screen_bg.y + y_pad + cursor_height); // Position verticale ajustée
        knob_label.destroy();
	label.destroy();
	screen.destroy();
	return;
}

void render::but(const std::vector<button*>& b, int x, int y, int pad)
{
    text label(this->r, 18, BASIC);
    int current_x = x;
    for (int i = 0; i < b.size(); i++)
    {
        int label_text_width = b[i]->label.size() * 18 * 0.5f;
        int label_center_x = current_x + (b[i]->size / 2) - (label_text_width / 2);
        label.write(b[i]->label, DARK_GRAY, label_center_x, y);
        b[i]->r = {current_x, y + 30, b[i]->size, b[i]->size};
        SDL_SetRenderDrawColor(this->r, B_SHADOW.r, B_SHADOW.g, B_SHADOW.b, B_SHADOW.a);
        SDL_RenderFillRect(this->r, &b[i]->r);
        SDL_Rect button_fg = {current_x + 7, b[i]->r.y + 5, b[i]->size - 15, b[i]->size - 15};
        SDL_SetRenderDrawColor(this->r, B_LIGHT.r, B_LIGHT.g, B_LIGHT.b, B_LIGHT.a);
        SDL_RenderFillRect(this->r, &button_fg);
        SDL_Rect button_led = {button_fg.x + 7, button_fg.y + 5, b[i]->size - 30, 7};
        SDL_SetRenderDrawColor(this->r, b[i]->state ? LED.r : BLUE_GRAY.r,
            b[i]->state ? LED.g : BLUE_GRAY.g, b[i]->state ? LED.b : BLUE_GRAY.b, BLUE_GRAY.a);
        SDL_RenderFillRect(this->r, &button_led);
        std::string l =
        b[i]->label == "1" ? "BD" : b[i]->label == "2" ? "SD" : b[i]->label == "3" ?
        "LT" : b[i]->label == "4" ? "MT" : b[i]->label == "5" ? "HT" : b[i]->label == "6" ?
        "RS" : b[i]->label == "7" ? "CLP" : b[i]->label == "8" ? "COH" : b[i]->label == "9" ?
        "OOH" : b[i]->label == "10" ? "CRH" : b[i]->label == "11" ? "RD" : " ";
        label_text_width = l.size() * 18 * 0.5f;
        label_center_x = current_x + (b[i]->size / 2) - (label_text_width / 2);
        label.write(l, DARK_GRAY, label_center_x, b[i]->r.y + b[i]->r.h + pad);
        current_x += b[i]->size + pad;
    }
    label.destroy();
}


void render::seq(sequencer* s)
{
	std::vector<button*> b;
	for (int i = 0; i < s->buttons.size(); i++)
	{
		b.push_back(&s->buttons[i]);
	}
	this->but(b, 20, 720, 15);
	return;
}

void render::show(const std::vector<module*>& modules, controller* c, int seq_disp)
{
        this->background();
        text model(this->r, 76, TR_FONT);
        model.write("NUOVO", BLUE_GRAY, 20, 50);
        text slogan(this->r, 36, TR_FONT);
        slogan.write("Rhythm Composer", BLUE_GRAY, 1150, 70);
        this->modules(modules);
	this->control(c);
	this->seq(modules[seq_disp]->s);
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
