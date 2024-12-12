#include "module.hpp"

module::module(std::string label, std::vector<knob> knobs, std::string file, int chan)
: label(label), knobs(knobs), chan(chan)
{
	if (file != "")
	{
		this->sample = Mix_LoadWAV(file.c_str());
		if (this->sample == nullptr) {std::cerr << Mix_GetError() << std::endl;}
	}
	else this->sample = nullptr;
	this->s = new sequencer();
	return;
}

module::~module()
{
	if (this->sample != nullptr) Mix_FreeChunk(this->sample);
	delete this->s;
	return;
}

std::string module::get_label() { return this->label; }

std::vector<knob>& module::get_knobs() { return this->knobs; }

void module::play()
{
	if (Mix_PlayChannel(this->chan, this->sample, 0) == -1) puts(Mix_GetError());
	return;
}
