#ifndef MODULE_HPP
#define MODULE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <filesystem>
#include <algorithm>

#include "models.hpp"
#include "sequencer.hpp"

class module
{
private:
    std::string label;
    std::vector<knob> knobs;
    int chan;
    std::map<std::string, Mix_Chunk*> samples;
    std::string build_sample_key();
	std::map<std::string, std::set<int>> available_values;
	void parse_samples(const std::string& folder);
public:
    module(std::string label, std::vector<knob> knobs, std::string folder, int chan);
    ~module();
	sequencer* s;
    std::string get_label();
    std::vector<knob>& get_knobs();
    sequencer* get_sequencer();
    void play();
    void set_chunk_volume();
};

#endif // MODULE_HPP
